
set serveroutput on

DROP TABLE log_note;
/

DROP SEQUENCE log_note_seq;
/

CREATE TABLE log_note(
    id INT NOT NULL,
    id_nota INT NOT NULL,
    valoare_veche INT,
    valoare_noua INT,
    tip_operatie VARCHAR2(7),
    log_user VARCHAR2(31),
    log_date DATE
);
/

ALTER TABLE log_note ADD(CONSTRAINT log_note_pk PRIMARY KEY (id));
/

CREATE SEQUENCE log_note_seq START WITH 1;
/

CREATE OR REPLACE TRIGGER log_note_pk
BEFORE INSERT ON log_note
FOR EACH ROW
BEGIN
    SELECT log_note_seq.NEXTVAL
    INTO :NEW.id
    FROM dual;
END;
/

CREATE OR REPLACE TRIGGER note_insert
AFTER INSERT ON note
FOR EACH ROW
DECLARE
    v_user VARCHAR2(32);
BEGIN
    SELECT USER INTO v_user FROM dual;
    INSERT INTO log_note (id_nota, valoare_veche, valoare_noua, tip_operatie, log_user, log_date)
        VALUES(:NEW.id, null, :NEW.valoare, 'INSERT', v_user, SYSDATE);
END;
/

CREATE OR REPLACE TRIGGER note_delete
AFTER DELETE ON note
FOR EACH ROW
DECLARE
    v_user VARCHAR2(32);
BEGIN
    SELECT USER INTO v_user FROM dual;
    INSERT INTO log_note (id_nota, valoare_veche, valoare_noua, tip_operatie, log_user, log_date)
        VALUES(:OLD.id, :OLD.valoare, null, 'DELETE', v_user, SYSDATE);
END;
/

CREATE OR REPLACE TRIGGER note_update
AFTER UPDATE OF valoare ON note
FOR EACH ROW
DECLARE
    v_user VARCHAR2(32);
BEGIN
    SELECT USER INTO v_user FROM dual;
    INSERT INTO log_note (id_nota, valoare_veche, valoare_noua, tip_operatie, log_user, log_date)
        VALUES(:NEW.id, :OLD.valoare, :NEW.valoare, 'UPDATE', v_user, SYSDATE);
END;
/

select * from log_note;
/

insert into note values(50000, 1, 24, 6, sysdate, sysdate, sysdate);
/

update note set valoare = 8 where id = 50000;
/

delete from note where id = 50000;
/