
CREATE OR REPLACE FUNCTION exception_div_a_b
RETURN INTEGER AS
BEGIN
    return 10 / 0;
END;
/

CREATE OR REPLACE FUNCTION exception_function_dml
RETURN INTEGER AS
BEGIN
    INSERT INTO profesori VALUES(1, 'Exceptie', 'Exceptie', 'Profesor', SYSDATE, SYSDATE);
    return 10;
END;
/

CREATE OR REPLACE FUNCTION exception_custom
RETURN INTEGER AS
    custom_exception EXCEPTION;
    PRAGMA EXCEPTION_INIT(custom_exception, -20001);
BEGIN
    raise custom_exception;
EXCEPTION
    WHEN custom_exception THEN
        raise_application_error(-20001, 'This exception was thrown by Alexandru Oloieri');
END;
/
