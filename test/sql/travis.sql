CREATE USER 'migrate'@'localhost' IDENTIFIED BY 'migrate';
GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP ON migrate.* TO 'migrate'@'localhost';
