We decided to use bcrypt for password hashing and salting:

npm install bcrypt


Then we need to manually create an 'admin' account then alter the row to set is_admin to true:

(UPDATE Users
SET is_admin = TRUE
WHERE username = 'admin';)


We also had to manually set a password for mySQL to work (this is already in the sql file):

ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'password';

Then, we accessed the mysql terminal using:

mysql -p Dodgyclubs

(then entering 'password')