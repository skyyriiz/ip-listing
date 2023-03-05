# Don't forget to give rights to the file: chmod +x requirements.sh
# Execute this file one time to install all the requirements

# Install sql server
sudo apt install mysql-server

# Install mysqlclient lib for c
sudo apt install default-libmysqlclient-dev 

# Configure sql server
echo "CREATE USER 'mike'@'localhost' IDENTIFIED BY 'jgldiff';" | sudo mysql
echo "User created"

echo "GRANT ALL PRIVILEGES ON * . * TO 'mike'@'localhost';" | sudo mysql
echo "Privileges granted"

echo "FLUSH PRIVILEGES;" | sudo mysql
echo "Privileges flushed"

echo "CREATE DATABASE ip;" | sudo mysql
echo "Database created"

echo "USE ip; CREATE TABLE ips (id INT NOT NULL AUTO_INCREMENT, ip VARCHAR(255) NOT NULL, PRIMARY KEY (id));" | sudo mysql
echo "Table created"

echo "USE ip; INSERT INTO ips (ip) VALUES ('192.168.200.1');" | sudo mysql
echo "USE ip; INSERT INTO ips (ip) VALUES ('192.168.200.2');" | sudo mysql
echo "USE ip; INSERT INTO ips (ip) VALUES ('192.168.220.1');" | sudo mysql
echo "USE ip; INSERT INTO ips (ip) VALUES ('192.168.220.2');" | sudo mysql
echo "Data inserted"

echo "exit;" | sudo mysql
echo "Exit"

echo "Done"



