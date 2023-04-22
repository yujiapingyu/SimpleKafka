# 从process.pid文件中读取进程号，然后使用kill -2命令杀掉进程
export WORKSPACE=$PWD
kill -2 `cat $WORKSPACE/process.pid`
rm -rf $WORKSPACE/process.pid
echo "process stopped!"