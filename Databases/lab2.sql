-- ex1
select nume, prenume, an, data_nastere from Studenti;
-- ex2 
select distinct bursa from Studenti where bursa is not null;
-- ex3
select nume as "a nume", prenume as "a prenume", an as "a an", data_nastere as "a data_nastere" from Studenti;
-- ex4
select nume||' '||prenume||', anul '||an as "Studenti pe ani de studiu" from Studenti order by an;
-- ex5
select nume, prenume, data_nastere from Studenti where data_nastere between '13-JAN-1995' and '10-JUN-1997' order by data_nastere;
-- ex6
select nume, prenume, an from Studenti where data_nastere between '1-JAN-1995' and '31-DEC-1995';
-- ex7
select * from Studenti where bursa is null;
-- ex8
select nume, prenume from Studenti where bursa is not null and an between 2 and 3 order by nume ASC, prenume DESC;
-- ex9
select Studenti.*, bursa * 1.15 as "Noua bursa" from Studenti where bursa is not null;
-- ex10
select * from Studenti where an = 1 and nume like 'P%';
-- ex11
select * from Studenti where nume like '%a%a%';
-- ex12
select * from Studenti where prenume in ('Alexandru', 'Ioana', 'Marius');
-- ex13
select * from Studenti where grupa like 'A_';
-- ex14
select * from Profesori where trim(nume) like '%n';
