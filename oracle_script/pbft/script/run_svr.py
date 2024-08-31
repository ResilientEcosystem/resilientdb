#!/bin/sh

import sys

from oracle_script.comm.utils import *
from oracle_script.comm.comm_config import *

cert_path="pbft_cert/"

def gen_svr_config(config):
    iplist=get_ips(config["svr_ip_file"])
    with open(config["svr_config_path"],"w") as f:
        for idx,ip in iplist:
            port = int(config["base_port"]) + int(idx)
            f.writelines("{} {} {}\n".format(idx, ip, port))


def kill_svr(config):
    run_cmd("sh /home/ubuntu/nexres/oracle_script/pbft/script/killall.sh {}".format(config["svr_ip_file"]))
    run_cmd("sh /home/ubuntu/nexres/oracle_script/pbft/script/killall.sh {}".format(config["cli_ip_file"]))
    print("kill done")

def run_svr(config):
    iplist=get_ips(config["svr_ip_file"])
    cli_iplist=get_ips(config["cli_ip_file"])
    print("cli:",cli_iplist)
    run_cmd("sh /home/ubuntu/nexres/oracle_script/pbft/script/run_server.sh {}".format(config["svr_ip_file"]))
    for (idx,svr_ip) in iplist+cli_iplist:
        private_key="{}/node_{}.key.pri".format(cert_path,idx)
        cert="{}/cert_{}.cert".format(cert_path,idx)
        if [idx,svr_ip] in iplist:
            continue
        else:
            cmd="nohup {} {} {} {} > client{}.log 2>&1 &".format(
		    get_remote_file_name(config["svr_bin_bazel_path"]),
		    get_remote_file_name(config["svr_config_path"]), 
		    private_key, 
		    cert, idx)
            print("cmd:",cmd)
        run_remote_cmd(svr_ip, cmd)

def upload_svr(config):
    iplist=get_ips(config["svr_ip_file"]) + get_ips(config["cli_ip_file"])
    rm_cmd=[]
    scp_cmd=[]
    cmd_list=[]
    for (idx,svr_ip) in iplist:
        private_key=config["cert_path"]+"/"+ "node_"+idx+".key.pri"
        cert=config["cert_path"]+ "/"+ "cert_"+idx+".cert"

        rm_cmd.append("\"ssh -i {} -n -o BatchMode=yes -o StrictHostKeyChecking=no ubuntu@{} \'rm -rf {}; rm -rf server.config; rm -rf cert; mkdir -p {};\' \"".format(KEY,svr_ip,get_remote_file_name(config["svr_bin_bazel_path"]), cert_path))

        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu\"".format(KEY,config["svr_bin_bazel_path"],svr_ip))
        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu\"".format(KEY,config["svr_config_path"],svr_ip))
        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu/{}\"".format(KEY,private_key, svr_ip, cert_path))
        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu/{}\"".format(KEY,cert, svr_ip, cert_path))

        #run_remote_cmd(svr_ip, "rm -rf {}; rm -rf server.config; rm -rf cert; mkdir -p {};".format(get_remote_file_name(config["svr_bin_bazel_path"]), cert_path))

        #cmd_list.append("scp -i {} {} ubuntu@{}:/home/ubuntu".format(KEY,config["svr_bin_bazel_path"],svr_ip))
        #cmd_list.append("scp -i {} {} ubuntu@{}:/home/ubuntu".format(KEY,config["svr_config_path"],svr_ip))
        #cmd_list.append("scp -i {} {} ubuntu@{}:/home/ubuntu/{}".format(KEY,private_key, svr_ip, cert_path))
        #cmd_list.append("scp -i {} {} ubuntu@{}:/home/ubuntu/{}".format(KEY,cert, svr_ip, cert_path))
        #run_cmd_with_resp("scp -i {} {} ubuntu@{}:/home/ubuntu".format(KEY,config["svr_bin_bazel_path"],svr_ip))
        #run_cmd_with_resp("scp -i {} {} ubuntu@{}:/home/ubuntu".format(KEY,config["svr_config_path"],svr_ip))
        #run_cmd_with_resp("scp -i {} {} ubuntu@{}:/home/ubuntu/{}".format(KEY,private_key, svr_ip, cert_path))
        #run_cmd_with_resp("scp -i {} {} ubuntu@{}:/home/ubuntu/{}".format(KEY,cert, svr_ip, cert_path))
    #run_remote_cmd_list_raw(cmd_list)
    cmd_shell = "/home/ubuntu/nexres/oracle_script/pbft/script/shell_cmd.sh"
    resp = run_cmd("sh {} {}".format(cmd_shell, ' '.join(rm_cmd)))
    resp = run_cmd("sh {} {}".format(cmd_shell, ' '.join(scp_cmd)))
    print("upload done")

if __name__ == '__main__':
    config_file=sys.argv[1]
    config = read_config(config_file)
    print("config:{}".format(config))
    gen_svr_config(config)
    kill_svr(config)
    upload_svr(config)
    run_svr(config)
