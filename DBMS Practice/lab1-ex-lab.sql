accept i_nume_curs char prompt 'Introdu numele cursului: '

set serveroutput on;
DECLARE
    v_nume_curs                 cursuri.titlu_curs%type;
    v_nota_maxima               note.valoare%type;
    v_numar_studenti            number(6);
    v_nume_student              studenti.nume%type;
    v_prenume_student           studenti.prenume%type;
    v_medie_generala_curs       double precision;
    v_rata_promovabilitate_curs double precision;
    v_numar_studenti_curs       number(6);
    v_numar_studenti_prom_curs  number(6);
    v_grupa_student             studenti.grupa%type;
    v_an_student                studenti.an%type;
    v_numar_colegi_student      number(6);
    v_numar_colegi_prom_student number(6);
BEGIN
    v_nume_curs := '&i_nume_curs';
    if (v_nume_curs is null)
        then return;
    end if;
    
    select max(n.valoare) into v_nota_maxima from note n join cursuri c on n.id_curs = c.id where c.titlu_curs = v_nume_curs;
    
    if (v_nota_maxima is null)
        then dbms_output.put_line('Cursul introdus nu exista!');
        else
            select count(distinct s.id) into v_numar_studenti from studenti s join note n on s.id = n.id_student join cursuri c on n.id_curs = c.id where c.titlu_curs = v_nume_curs and n.valoare = v_nota_maxima;
            dbms_output.put_line('Nota maxima la materia '||v_nume_curs||' este '||v_nota_maxima);
            dbms_output.put_line('Numarul de studenti ce au luat aceasta nota: '||v_numar_studenti);
            select * into v_nume_student, v_prenume_student, v_grupa_student, v_an_student from (select s.nume, s.prenume, s.grupa, s.an from studenti s join note n on s.id = n.id_student join cursuri c on n.id_curs = c.id where rownum <= 1 and c.titlu_curs = v_nume_curs and n.valoare = v_nota_maxima group by s.id, s.nume, s.prenume, s.an, s.grupa having avg(n.valoare) = (select max(avg(n.valoare)) from studenti s join note n on s.id = n.id_student join cursuri c on n.id_curs = c.id where c.titlu_curs = v_nume_curs and n.valoare = v_nota_maxima group by s.id));
            dbms_output.put_line('Studentul cu cea mai mare medie '||v_nume_student||' '||v_prenume_student);
        
            select avg(n.valoare) into v_medie_generala_curs from cursuri c join note n on n.id_curs = c.id where c.titlu_curs = v_nume_curs;
            dbms_output.put_line('Media generala a studentilor la materia '||v_nume_curs||' este '||v_medie_generala_curs);
            
            select count(distinct s.id) into v_numar_studenti_curs from studenti s join note n on s.id = n.id_student join cursuri c on c.id = n.id_curs where c.titlu_curs = v_nume_curs;
            select count(*) into v_numar_studenti_prom_curs from (select 1 from studenti s join note n on s.id = n.id_student join cursuri c on c.id = n.id_curs where c.titlu_curs = v_nume_curs group by s.id having avg(n.valoare) >= 5);
            dbms_output.put_line('Rata de promovabilitate la materia '||v_nume_curs||' este '||100*v_numar_studenti_prom_curs/v_numar_studenti_curs||'%');
         
            select count(distinct s.id) into v_numar_colegi_student from studenti s join note n on s.id = n.id_student join cursuri c on c.id = n.id_curs where c.titlu_curs = v_nume_curs and s.grupa = v_grupa_student and s.an = v_an_student;
            select count(*) into v_numar_colegi_prom_student from (select 1 from studenti s join note n on s.id = n.id_student join cursuri c on c.id = n.id_curs where c.titlu_curs = v_nume_curs and s.grupa = v_grupa_student and s.an = v_an_student group by s.id having avg(n.valoare) >= 5);
            dbms_output.put_line('Rata de promovabilitate la materia '||v_nume_curs||' a colegilor studentului '||v_nume_student||' '||v_prenume_student||' este '||100*v_numar_colegi_prom_student/v_numar_colegi_student||'%');
            
    end if;
END;

/*
Enunt:

In momentul rularii scriptului veti fi intrebati un 
nume de curs. Daca acel curs exista in baza de date,
se vor afisa pentru el care este studentul care a luat
nota cea mai mare. Daca sunt mai multi se vor afisa cati sunt, si primul in ordinea mediilor (doar numele si prenumele).

Construiti inca 3 statistici la alegere pentru acel curs (de exemplu care este media generala a studentilor, rata de promovabilitate, etc.)

NEAPARAT: folositi %type pentru declararea variabilelor in care preluati date din BD
*/
