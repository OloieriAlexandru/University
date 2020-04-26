
-- Masinile sunt sortate dupa "varsta" (numarul de ani)
CREATE OR REPLACE TYPE masina AS OBJECT
(
    marca VARCHAR2(15),
    model VARCHAR2(20),
    an_fabricatie INT,
    culoare VARCHAR2(10),
    motorizare VARCHAR2(10),
    CONSTRUCTOR FUNCTION masina(marca VARCHAR2, model VARCHAR2, an_fabricatie INT)
        RETURN SELF AS RESULT,
    NOT FINAL MEMBER PROCEDURE afiseaza_detalii,
    MEMBER PROCEDURE vopseste(p_culoare VARCHAR2),
    MAP MEMBER FUNCTION ofera_numar_ani RETURN INT
) NOT FINAL;
/

CREATE OR REPLACE TYPE BODY masina AS
    CONSTRUCTOR FUNCTION masina(marca VARCHAR2, model VARCHAR2, an_fabricatie INT)
        RETURN SELF AS RESULT
    AS
    BEGIN
        SELF.marca := marca;
        SELF.model := model;
        SELF.an_fabricatie := an_fabricatie;
        SELF.culoare := '-';
        SELF.motorizare := '-';
        RETURN;
    END;
    NOT FINAL MEMBER PROCEDURE afiseaza_detalii
    AS
    BEGIN
        DBMS_OUTPUT.put_line('Masina: '||SELF.marca||' '||SELF.model||', an: '||SELF.an_fabricatie||', culoare: '||SELF.culoare||', motor: '||SELF.motorizare);
    END afiseaza_detalii;
    MEMBER PROCEDURE vopseste(p_culoare VARCHAR2)
    AS
    BEGIN
        SELF.culoare := p_culoare;
    END vopseste;
    MAP MEMBER FUNCTION ofera_numar_ani
    RETURN INT
    AS
    BEGIN
        RETURN FLOOR(EXTRACT(year from SYSDATE) - SELF.an_fabricatie);
    END ofera_numar_ani;
END;
/

CREATE OR REPLACE TYPE camioneta UNDER masina
(
    capacitate_platforma INT,
    OVERRIDING MEMBER PROCEDURE afiseaza_detalii,
    MEMBER PROCEDURE schimba_platforma (p_capacitate INT)
);
/

CREATE OR REPLACE TYPE BODY camioneta AS
    OVERRIDING MEMBER PROCEDURE afiseaza_detalii
    IS
    BEGIN
        DBMS_OUTPUT.put_line('Camioana: '||SELF.marca||' '||SELF.model||', an: '||SELF.an_fabricatie||', culoare: '||SELF.culoare||', motor: '||SELF.motorizare||', capacitate: '||SELF.capacitate_platforma||' kg');
    END afiseaza_detalii;
    MEMBER PROCEDURE schimba_platforma(p_capacitate INT)
    IS
    BEGIN
        SELF.capacitate_platforma := p_capacitate;
    END schimba_platforma;
END;
/

ALTER TABLE profesori
ADD masina masina;
/

/*
In blocul anonim fac urmatoarele operatii:
1. Generez pentru fiecare profesor cate o masina
2. Parcurg profesorii cu ajutorul unui cursor si afisez (nume,prenume,ani_masina), profesorii fiind sortati dupa coloana masina (pentru a demonstra ca obiectele pot fi intr-adevar comparate)
3. Creez 2 obiecte diferite de tip "masina", unul folosind constructorul implicit si unul folosind constructorul declarat de mine, iar apoi, pentru unul dintre cele 2 obiecte, apelez toate metodele implementate
4. Creez un obiect de tip "camioneta" pentru a demonstra faptul ca pot fi create obiecte din clasa derivata
----
Functionalitatea de generare a unei masini am luat-o din tema 4 (cea cu colectii), adica in tema 4 am folosit cam aceleasi reguli de generare a masinilor
*/
set serveroutput on;
DECLARE
    TYPE varr IS VARRAY(50) OF VARCHAR2(255);

    TYPE info_masina IS RECORD(
        marca VARCHAR2(15),
        modele varr
        );
    
    TYPE varr_m IS VARRAY(50) OF info_masina;
    masini varr_m;
    
    v_masina_1 masina;
    v_masina_2 masina;
    v_camioneta camioneta;
    
    culori varr := varr('neagra', 'alba', 'gri', 'rosie', 'verde', 'portocalie', 'galbena', 'visinie');
    motoare varr := varr('motorina', 'benzina');

    v_idx_masina INTEGER;
    v_marca VARCHAR2(15);
    v_model VARCHAR2(20);
    v_an_fabricatie INTEGER;
    v_culoare VARCHAR2(10);
    v_motor VARCHAR2(10);
    v_masina masina;
    
    CURSOR update_masini IS
        SELECT * FROM profesori FOR UPDATE OF masina NOWAIT;
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

    FOR v_linie IN update_masini LOOP
        v_idx_masina := TRUNC(DBMS_RANDOM.value(0, masini.COUNT)+1);
        v_marca := masini(v_idx_masina).marca;
        v_model := masini(v_idx_masina).modele(TRUNC(DBMS_RANDOM.value(0, masini(v_idx_masina).modele.COUNT)+1));
    
        v_an_fabricatie := TRUNC(DBMS_RANDOM.value(1999, 2018)+1);
        v_culoare := culori(TRUNC(DBMS_RANDOM.value(0, culori.COUNT)+1));
        v_motor := motoare(TRUNC(DBMS_RANDOM.value(0, motoare.COUNT)+1));

        v_masina := masina(v_marca, v_model, v_an_fabricatie, v_culoare, v_motor);     
        
        UPDATE profesori SET profesori.masina = v_masina WHERE CURRENT OF update_masini;
    END LOOP;
    
    DBMS_OUTPUT.put_line('Profesorii sortati dupa masina:');
    FOR v_prof IN (SELECT * FROM profesori ORDER BY masina ASC) LOOP -- ascending, cele mai noi masini primele (cu anul cel mai mare)
        DBMS_OUTPUT.put_line(v_prof.nume||' '||v_prof.prenume||' '||v_prof.masina.ofera_numar_ani());
    END LOOP;
    
    v_masina_1 := masina('Ford', 'Focus', 2015); -- constructorul declarat de mine
    v_masina_2 := masina('Ford', 'Focus', 2016, 'alba', 'benzina'); -- constructorul implicit
    
    v_masina_2.afiseaza_detalii();

    DBMS_OUTPUT.put_line('Masina 1 inainte de a fi vopsita:');
    v_masina_1.afiseaza_detalii();    
    DBMS_OUTPUT.put_line('Masina 1 dupa ce a fost vopsita:');
    v_masina_1.vopseste('rosie');
    v_masina_1.afiseaza_detalii();
    DBMS_OUTPUT.put_line('Varsta masina 1: '||v_masina_1.ofera_numar_ani());
    
    v_camioneta := camioneta('Fiat', 'Ducato', 2010, 'alba', 'motorina', 2500); -- constructor implicit
    DBMS_OUTPUT.put_line('Camioneta cu vechea platforma:');
    v_camioneta.afiseaza_detalii(); -- metoda suprascrisa
    DBMS_OUTPUT.put_line('Camioneta cu noua platforma:');
    v_camioneta.schimba_platforma(3500);
    v_camioneta.afiseaza_detalii();
    DBMS_OUTPUT.put_line('Varsta camioneta: '||v_camioneta.ofera_numar_ani());
END;
/