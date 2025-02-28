CREATE DATABASE ServerData;

USE ServerData;

create table user
(
    name varchar(128) not null primary key,
    data     text         null, -- <register_day>|<bcrypt_pwd>
    nickname varchar(64)  null
);