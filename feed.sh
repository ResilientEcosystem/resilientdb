for((i=0;i<5000000;i++))
do
bazel-bin/application/poc/pbft_client_main oracle_script/pbft/rep_4/client.config $i
done
