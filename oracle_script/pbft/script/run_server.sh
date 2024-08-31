input=$1
#input="/home/ubuntu/nexres/oracle_script/pow/rep_120/svr_list.txt"

count=0

HOSTS=`cat ${input}`
i=0
id=0
set -x
for HOSTNAME in ${HOSTS}; do
	if [ $i -eq 1 ]; then
		ssh -i /home/ubuntu/nexres/oracle_script/ssh-2022-03-24.key -n -o BatchMode=yes -o StrictHostKeyChecking=no ubuntu@${HOSTNAME} " nohup /home/ubuntu/pbft_server /home/ubuntu/server.config pbft_cert//node_${id}.key.pri pbft_cert//cert_${id}.cert > server${id}.log 2>&1 & "
		i=0
	else
		id=${HOSTNAME}
		i=1
	fi
	count=`expr $count + 1`
done

while [ $count -gt 0 ]; do
	wait $pids
	count=`expr $count - 1`
done
