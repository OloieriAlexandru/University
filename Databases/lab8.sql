-- ex1
select an, count(*) as NUMAR_STUDENTI from studenti group by an;
-- ex2
select an, grupa, count(*) from studenti group by an, grupa order by an, grupa;
-- ex3
select grupa, an, count(an) as NumarStudenti, count(bursa) from studenti group by an, grupa;
-- ex4
select sum(bursa) from studenti;
-- ex5
select sum(bursa)/count(*) as MEDIE_BURSA from studenti;
-- ex5v2
select avg(nvl(bursa,0)) as MEDIE_BURSA from studenti;
-- ex6
select valoare as nota, count(valoare) as numar from note group by valoare order by valoare;
-- ex7
select count(*) numar_note, to_char(data_notare, 'Day') zi_din_saptamana from note group by to_char(data_notare, 'Day') order by 1 desc;
-- ex8
select count(*) numar_note, to_char(data_notare, 'Day') zi_din_saptamana from note group by to_char(data_notare, 'Day'), to_char(data_notare, 'd') order by to_char(data_notare, 'd') asc;
-- ex9
select nume, avg(valoare) as Medie from studenti natural join note group by nr_matricol, nume order by 2;
-- ex10
select nume, avg(valoare) as Medie from studenti left outer join note on studenti.nr_matricol = note.nr_matricol group by studenti.nr_matricol, nume order by 2;
-- ex11
select nume, avg(NVL(valoare,0)) as Medie from studenti left outer join note on studenti.nr_matricol = note.nr_matricol group by studenti.nr_matricol, nume order by 2;
-- ex12
select nume, avg(NVL(valoare,0)) as Medie from studenti left outer join note on studenti.nr_matricol = note.nr_matricol group by studenti.nr_matricol, nume having avg(NVL(valoare,0)) > 8 order by 2;
-- ex13
select s.nume, max(valoare) nota_maxima, min(valoare) nota_minima, avg(valoare) medie from studenti s natural join note group by nr_matricol, nume having min(valoare) >= 7;
-- ex14
select nume, avg(valoare) from studenti natural join note group by nr_matricol, nume having count(valoare) > 3;
-- ex15
select nume, avg(valoare) from studenti natural join note where grupa = 'A2' and an = 3 group by nr_matricol, nume;
-- ex16
select max(avg(valoare)) as HIGHEST_MEAN from studenti natural join note group by nr_matricol, nume;
-- ex17
select id_curs, titlu_curs, min(valoare), max(valoare) from cursuri natural join note group by id_curs, titlu_curs;
-- ex17v2
select c.id_curs, titlu_curs, min(nvl(valoare,0)), max(nvl(valoare,0)) from cursuri c left outer join note n on n.id_curs = c.id_curs group by c.id_curs, titlu_curs;
