#源镜像
FROM mountainfeng/linux-0.11:latest
# FROM alpine:3.7
#作者
MAINTAINER mountainfpf "mountainfpf@gmail.com"

ENV TZ "Asia/Shanghai"

#设置工作目录
# WORKDIR $GOPATH/src/github.com/fengpf/job
WORKDIR /linux-0.11
#将服务器的go工程代码加入到docker容器中
ADD ./linux-0.11 ./
#go构建可执行文件
# CMD go version && go build -v
# RUN go build .
#暴露端口
EXPOSE 8080
#最终运行docker的命令
ENTRYPOINT  [""]