FROM php:7.4-apache

RUN apt-get update && apt upgrade -y
RUN docker-php-ext-install mysqli pdo pdo_mysql && docker-php-ext-enable mysqli

WORKDIR /var/www/html
COPY . .

EXPOSE 80
