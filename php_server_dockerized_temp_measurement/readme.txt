istallation

1. install docker
2. cd to this folder and run commands below
3. docker-compose build --no-cache
4. docker-compose up -d
5. open phpmyadmin (http://localhost:8234/) , login to db (host: mysql_db, user: admin, pasw: admin), select db databse and import table from temp.sql file
6. check connection on http://localhost:88/
7. !!! this folder share file with docker containers so dont delete or move it !!!
