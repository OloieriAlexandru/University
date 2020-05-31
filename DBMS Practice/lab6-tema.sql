
DROP TABLE t_masini;
/

CREATE TABLE t_masini
(
    id INT PRIMARY KEY,
    masina masina
);
/

select * from t_masini;
/

CREATE OR REPLACE PROCEDURE serializeaza_masina(p_id INT, p_marca VARCHAR2, p_model VARCHAR2, p_an INT, p_culoare VARCHAR2, p_motor VARCHAR2)
AS
    v_masina masina;
    v_numar INT;
BEGIN
    v_masina := masina(p_marca, p_model, p_an, p_culoare, p_motor);
        
    SELECT COUNT(*) INTO v_numar FROM t_masini WHERE id = p_id;
    IF v_numar = 0 THEN
        INSERT INTO t_masini VALUES(p_id, v_masina);
    ELSE
        UPDATE t_masini SET t_masini.masina = v_masina WHERE t_masini.id = p_id;
    END IF;
END;
/

CREATE OR REPLACE PROCEDURE deserializeaza_masina(p_id INT, p_marca OUT VARCHAR2, p_model OUT VARCHAR2, p_an OUT INT, p_culoare OUT VARCHAR2, p_motor OUT VARCHAR2)
AS
    v_masina masina;
BEGIN
    SELECT masina INTO v_masina FROM t_masini WHERE id = p_id;
    p_marca := v_masina.marca;
    p_model := v_masina.model;
    p_an := v_masina.an_fabricatie;
    p_culoare := v_masina.culoare;
    p_motor := v_masina.motorizare;
END;
/
