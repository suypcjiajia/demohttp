#!/bin/bash　　
#创建demo项目数据库

HOSTNAME="127.0.0.1"     #mysql host
PORT="3306"              #mysql port
USERNAME="dl"            #mysql user
PASSWORD="dl123456"      #mysql password
DBNAME="demo"            #mysql database


#也可以写 HOSTNAME="localhost"

echo "start..."
echo "HOSTNAME:$HOSTNAME PORT:$PORT USERNAME:$USERNAME PASSWORD:$PASSWORD DBNAME:$DBNAME"

 #创建数据库
sql="create database IF NOT EXISTS ${DBNAME}"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD} -e "${sql}"

#创建用户表
sql="create table IF NOT EXISTS user
(
  account VARCHAR(12) NOT NULL  comment '注册手机号码',
  password VARCHAR(64) NOT NULL  comment '密码',
  sex int NOT NULL comment '姓别0-女 1-男 2-未知',
  headImage VARCHAR(200) NOT NULL  comment '头像',
  nickName VARCHAR(64) NOT NULL  comment '昵称',
  PRIMARY KEY(account)
) comment='用户表' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"



