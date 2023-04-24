# 判断平台是arm的还是amd的
if [ $(uname -m) = "x86_64" ]; then
    echo "Ploatform: amd64"
    ./tool/bazel_amd build :all --cxxopt="-Wno-narrowing"
else
    echo "Ploatform: arm"
    ./tool/bazel_arm build :all --cxxopt="-Wno-narrowing"
fi

cp -f ./bazel-bin/main_work ./main_work