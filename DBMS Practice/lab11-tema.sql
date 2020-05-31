
-- Oracle query: 

DROP TABLE employees;
/

DROP SEQUENCE employees_seq;
/

CREATE TABLE employees (
    id INT PRIMARY KEY,
    first_name VARCHAR2(255),
    last_name VARCHAR2(255)
);
/

CREATE SEQUENCE employees_seq;
/

CREATE OR REPLACE TRIGGER employees_on_insert
    BEFORE INSERT ON employees
    FOR EACH ROW
BEGIN
    SELECT employees_seq.nextval
    INTO :new.id
    FROM dual;
END;
/

/*

MySQL query:

CREATE TABLE employees (
    id INT(6) PRIMARY KEY AUTO_INCREMENT,
    first_name VARCHAR(255),
    last_name VARCHAR(255)
    );

*/