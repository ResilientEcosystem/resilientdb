#set -x
iplist=(
10.2.0.64
10.2.0.30
10.2.0.71
10.2.0.215
10.2.0.46
10.2.0.213
10.2.0.22
10.2.0.156
10.2.0.162
10.2.0.115
10.2.0.47
10.2.0.7
10.2.0.138
10.2.0.169
10.2.0.27
10.2.0.235
)

count=0
for ip in ${iplist[@]}
do
	ssh -i ~/.ssh/ssh-2022-03-24.key -n -o BatchMode=yes -o StrictHostKeyChecking=no ubuntu@${ip} "cd /home/ubuntu; grep 'execute seq' pow_server*.log | grep lat"
        ((count++))
done

