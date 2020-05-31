
DROP TABLE masini;
/

CREATE TABLE masini (
    id INT NOT NULL PRIMARY KEY,
    id_student INT NOT NULL,
    marca VARCHAR2(15) NOT NULL,
    model VARCHAR2(20) NOT NULL,
    an_fabricatie INT,
    culoare VARCHAR2(10),
    motor VARCHAR2(10)
);
/

DECLARE
    TYPE varr IS VARRAY(50) OF VARCHAR2(255);

    TYPE info_masina IS RECORD(
        marca VARCHAR2(15),
        modele varr
        );
    
    TYPE varr_m IS VARRAY(50) OF info_masina;
    masini varr_m;
    
    culori varr := varr('neagra', 'alba', 'gri', 'rosie', 'verde', 'portocalie', 'galbena', 'visinie');
    motoare varr := varr('diesel', 'benzina');
    
    v_studenti_numar INTEGER;
    
    v_idx_masina INTEGER;
    
    v_id_student INTEGER;
    v_marca VARCHAR2(15);
    v_model VARCHAR2(20);
    v_an_fabricatie INTEGER;
    v_culoare VARCHAR2(10);
    v_motor VARCHAR2(10);
BEGIN
    masini := varr_m();
    
    masini.EXTEND;
    masini(masini.LAST).marca := 'Dacia';
    masini(masini.LAST).modele := varr('Logan', 'Duster', 'Sandero', 'Lodgy', 'Logan MCV Stepway', 'Sandero Stepway');
    
    masini.EXTEND;
    masini(masini.LAST).marca := 'BWM';
    masini(masini.LAST).modele := varr('X1', 'X2', 'X3', 'X4', 'X5', 'X6', 'X7', 'i8', '3 Series' ,'4 Series', '5 Series', '6 Series', '7 series', 'M4', 'M5');
    
    masini.EXTEND;
    masini(masini.LAST).marca := 'Mercedes';
    masini(masini.LAST).modele := varr('C 63 S', 'SL-Class', 'GLC-Class', 'S-Class', 'CLS 350', 'CLS 320');
    
    masini.EXTEND;
    masini(masini.LAST).marca := 'Audi';
    masini(masini.LAST).modele := varr('A1', 'A2', 'A3', 'A4', 'A5', 'A6', 'A7', 'A8', 'Q1', 'Q2', 'Q3', 'Q4', 'Q5', 'Q6', 'Q7', 'R8');
    
    masini.EXTEND;
    masini(masini.LAST).marca := 'Volkswagen';
    masini(masini.LAST).modele := varr('Golf 1', 'Golf 2', 'Golf 3', 'Golf 4', 'Golf 5', 'Golf 6');
    
    SELECT COUNT(*) INTO v_studenti_numar FROM studenti;
    
    FOR v_i IN 1..200 LOOP
    
        v_id_student := TRUNC(DBMS_RANDOM.value(0, v_studenti_numar)+1);
    
        v_idx_masina := TRUNC(DBMS_RANDOM.value(0, masini.COUNT)+1);
        v_marca := masini(v_idx_masina).marca;
        v_model := masini(v_idx_masina).modele(TRUNC(DBMS_RANDOM.value(0, masini(v_idx_masina).modele.COUNT)+1));
    
        v_an_fabricatie := TRUNC(DBMS_RANDOM.value(1999, 2018)+1);
        v_culoare := culori(TRUNC(DBMS_RANDOM.value(0, culori.COUNT)+1));
        v_motor := motoare(TRUNC(DBMS_RANDOM.value(0, motoare.COUNT)+1));
    
        INSERT INTO masini VALUES (v_i, v_id_student, v_marca, v_model, v_an_fabricatie, v_culoare, v_motor);
    END LOOP;
    
END;
/

select * from masini;
/

SELECT * from studenti left outer join masini on studenti.id = masini.id_student;
/
