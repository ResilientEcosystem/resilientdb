#!/bin/sh

import sys
import time
from datetime import datetime


from oracle_script.comm.utils import *
from oracle_script.comm.comm_config import *


def get_avg(config):
    iplist=get_ips(config["svr_ip_file"])
    count = {}
    avg_time = []
    tot = 0
    lat = 0
    num = 0
    for (idx,svr_ip) in iplist:
        cmd="grep \'minging time:\' /home/ubuntu/pow_server{}.log".format(idx)
        res=run_remote_cmd(svr_ip, cmd)
        for r in res:
            rep=r.decode('utf8')
            print("re:{}".format(rep))
            if len(rep)>0:
        #data = res.split(':')[-1]
                print("re:{}".format(rep))
                #print("get res:",rep[5], rep[6])
                #lat += int(rep[5])
                #num += int(rep[6])

        #if num > 0:
        #    print("avg time:",lat/num)
        #tot += float(data)
    #print("avg time:",lat/num)

    '''
    tp = 0
    tp_num = 0
    for (idx,svr_ip) in iplist:
        cmd="grep \'update block:\' /home/ubuntu/pow_server{}.log".format(idx)
        res=run_remote_cmd(svr_ip, cmd)
        for r in res:
            rep=r.decode('utf8').split(':')
            if len(rep)>1:
                print("get res:",rep[1])
                tp += int(rep[1])
                tp_num +=1

        #tot += float(data)
    print("tput :",tp/tp_num)
    '''


def get_block_time(config):
    iplist=get_ips(config["svr_ip_file"])
    count = {}
    avg_time = []
    total_time = []
    for (idx,svr_ip) in iplist:
        cmd="grep \'total commit:\' /home/ubuntu/pow_server{}.log | grep avg | tail -1".format(idx)
        res=run_remote_cmd(svr_ip, cmd)
        res=res[0].decode('utf8')
        data = res.split()
        print("res:",data)
        avg_time.append(data[-1].split(':')[-1])
        total_time.append(data[-3].split(':')[-1])
    print("avg commit:",avg_time)
    print("avg commit:",total_time)

def get_min_block(config):
    iplist=get_ips(config["svr_ip_file"])
    count = {}
    commit_count = {}
    for (idx,svr_ip) in iplist:
        cmd="grep \'mine block successful, id\' /home/ubuntu/pow_server{}.log".format(idx)
        res=run_remote_cmd(svr_ip, cmd)
        if(len(res)):
            l = []
            for s in res:
                seq=s.decode('utf8').split(':')
                print("result:seq{}".format(seq))
                if(len(seq)):
                    d = seq[-1].strip()
                    l.append(d)
                    if d not in count:
                        count[d]=1
                    else:
                        count[d] = count[d]+1
                        print("{} is re-committed".format(d))
            commit_count[idx] = len(l)
    print("commit per:",commit_count)
    print("=====================")

    avg_time = []
    for (idx,svr_ip) in iplist:
        cmd="grep \'commit new block\' /home/ubuntu/pow_server{}.log".format(idx)
        res=run_remote_cmd(svr_ip, cmd)
        if(len(res)):
            delta = 0
            last = 0
            for s in res:
                t = str(s.split()[1].strip().decode('utf8'))
                print("s:",t.split(':'))
                if(len(t.split(':'))<3):
                    print("!!!!! no data")
                    continue
                if(t.split(':')[0]=="23"):
                    t = "2022-04-01 {}".format(t)
                else:
                    t = "2022-04-02 {}".format(t)
                datetime_obj = datetime.strptime(t, "%Y-%m-%d %H:%M:%S.%f")
                stamp = int(time.mktime(datetime_obj.timetuple()) * 1000.0 + datetime_obj.microsecond / 1000.0)
                if last > 0:
                    delta += stamp - last
                    print("last :{} cur:{}, delta:{}, time:{}".format(last, stamp, delta, t))
                last = stamp
            avg_time.append(delta/(len(res)-1)/1000)
    print("avg_time:",sum(avg_time)/len(avg_time))

def stat(config):
    #get_avg(config)
    #get_block_time(config)
    get_min_block(config)

if __name__ == '__main__':
    config_file=sys.argv[1]
    config = read_config(config_file)
    #stat(config)
    get_avg(config)

