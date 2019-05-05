#!/bin/bash　　
#创建院感项目数据库

HOSTNAME="127.0.0.1"     #mysql host
PORT="3306"              #mysql port
USERNAME="dl"            #mysql user
PASSWORD="dl123456"      #mysql password
DBNAME="hospital"            #mysql database


#也可以写 HOSTNAME="localhost"

echo "start..."
echo "HOSTNAME:$HOSTNAME PORT:$PORT USERNAME:$USERNAME PASSWORD:$PASSWORD DBNAME:$DBNAME"

 #创建数据库
sql="create database IF NOT EXISTS ${DBNAME}"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD} -e "${sql}"

#创建用户表
sql="create table IF NOT EXISTS user
(
  id int auto_increment  PRIMARY KEY comment '用户编号',
  name VARCHAR(64) NOT NULL unique key comment '用户姓名',
  department VARCHAR(64) NOT NULL  comment '用户科室',
  password VARCHAR(64) NOT NULL  comment '用户密码',
  level int NOT NULL default 0 comment '用户级别 0-normal  999-admin'
) comment='用户表' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"


#创建设置表
sql="create table IF NOT EXISTS setting
(
  id int auto_increment  PRIMARY KEY comment '编号',
  name VARCHAR(64) NOT NULL comment '名称',
  type int NOT NULL comment '类型 0-医院名称 1-科室 2-采样者 3-检验者 4-审核者 5-致病菌',
  unique key(name,type)
) comment='设置表' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"

#创建标准录入表
sql="create table IF NOT EXISTS standardInput
(
  id int auto_increment  PRIMARY KEY comment '编号',
  type int NOT NULL comment '类型 0-环境空气 1-物体表面 2-医务人员手 3-消毒剂及器械保存液 4-透析液 5-消毒灭菌效果监测 6-污水排放 7-医疗用品',
  value0 VARCHAR(128) NOT NULL comment '录入值0',
  value1 VARCHAR(128) NOT NULL comment '录入值1',
  value2 VARCHAR(128) NOT NULL comment '录入值2',
  value3 VARCHAR(128) NOT NULL comment '录入值3',
  value4 VARCHAR(128) NOT NULL comment '录入值4',
  value5 VARCHAR(128) NOT NULL comment '录入值5'
) comment='标准录入' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"


#创建录入表
sql="create table IF NOT EXISTS records
(
  id int auto_increment  PRIMARY KEY comment '编号',
  type int NOT NULL comment '类型 0-环境空气 1-物体表面 2-医务人员手 3-消毒剂及器械保存液 4-透析液 5-消毒灭菌效果监测 6-污水排放 7-医疗用品',
  collectTime datetime comment '采样日期',
  monitorTime datetime comment '监测日期',
  reportTime datetime comment '报告日期',
  department VARCHAR(64) NOT NULL  comment '被监测科室',
  collector VARCHAR(64) NOT NULL  comment '采样者',
  varifior VARCHAR(64) NOT NULL  comment '检验者',
  checkor VARCHAR(64) NOT NULL  comment '审核者',
  value0 VARCHAR(128) NOT NULL comment '录入值0',
  value1 VARCHAR(128) NOT NULL comment '录入值1',
  value2 VARCHAR(128) NOT NULL comment '录入值2',
  value3 VARCHAR(128) NOT NULL comment '录入值3',
  value4 VARCHAR(128) NOT NULL comment '录入值4',
  value5 VARCHAR(128) NOT NULL comment '录入值5',
  value6 VARCHAR(128) NOT NULL comment '录入值6',
  value7 VARCHAR(128) NOT NULL comment '录入值7',
  value8 VARCHAR(128) NOT NULL comment '录入值8',
  value9 VARCHAR(128) NOT NULL comment '录入值9',
  value10 VARCHAR(128) NOT NULL comment '录入值10',
  value11 VARCHAR(128) NOT NULL comment '录入值11',
  value12 VARCHAR(128) NOT NULL comment '录入值12',
  value13 VARCHAR(128) NOT NULL comment '录入值13',
  value14 VARCHAR(128) NOT NULL comment '录入值14',
  value15 VARCHAR(128) NOT NULL comment '录入值15',
  value16 VARCHAR(128) NOT NULL comment '录入值16',
  value17 VARCHAR(128) NOT NULL comment '录入值17'
) comment='录入' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"
