# linux-0.11

###1.1 build by Ubuntu

### 生成镜像
docker build -t mountainfeng/linux:0.11 .

### 单步调试查看log
docker run -it --name linux-0.11 mountainfeng/linux:0.11

### 容器退出删掉
docker run -d --rm --name linux-0.11 mountainfeng/linux:0.11


### 打标签
docker tag mountainfeng/linux:0.11 mountainfeng/linux-0.11:v1

### 将当前镜像发送到远端 hub.docker.com
docker push mountainfeng/linux-0.11:v1


###导出容器
docker export -o linux-0.11.zip 89c16153c168
###导入容器
docker import linux-0.11.zip mountainfeng/linux-0.11:v1

###下面的命令启动一个bash终端，允许用户进行交互;按ctrl+d或输入exit命令来退出容器
docker run -it mountainfeng/linux-0.11:v1 /bin/bash  

###exec进入容器，并启动bash ； -i参数来保持标准输入打开，-t并且分配一个伪终端，-d后台运行
docker exec -it 453698741 bash

###基于已有的docker容器，做一新的dokcer image.
docker commit <container_id> <image_name>

### 查看network的名称，执行 
docker network list