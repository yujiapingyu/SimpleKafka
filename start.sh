export WORKSPACE=$PWD
echo "WORKSPACE: $WORKSPACE"

# 检查当前目录下是否存在 process.pid 文件，如果存在则说明程序已经启动，输出pid后直接退出
if [ -f "$WORKSPACE/process.pid" ]; then
    echo "consumer already started, pid = [`cat $WORKSPACE/process.pid`]."
    echo -e "\033[0;31myou can run stop.sh to stop it first.\033[0m"
    echo "now exit..."
    exit 1
fi

# 启动程序，并将pid写入process.pid文件
python3 -u $WORKSPACE/src/python/main.py &
pid=$!
echo "=============================================================="
echo -e "pid=\033[0;32m$pid\033[0m"
echo $pid > $WORKSPACE/process.pid