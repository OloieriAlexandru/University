1. select s1.nume, s2.nume, abs(s1.data_nastere - s2.data_nastere) as diferenta_varsta_zile 
	from studenti s1 cross join studenti s2 
	where s1.nr_matricol < s2.nr_matricol 
	order by 3;
2. select 'Studentul '||s.nume||' '||s.prenume||' ar putea fi prieten cu profesorul '||trim(p.nume)||' '||trim(p.prenume) as Prietenii 
	from studenti s cross join profesori p 
	where length(s.nume) = length(trim(p.nume));
3a. select distinct titlu_curs 
	from cursuri c 
	join note n on c.id_curs = n.id_curs 
	minus 
	select distinct titlu_curs 
	from cursuri c join note n on c.id_curs = n.id_curs 
	where n.valoare > 8;                        
3b. select c.titlu_curs, max(n.valoare) as nota_maxima 
	from cursuri c 
	join note n on c.id_curs = n.id_curs 
	group by titlu_curs 
	having max(n.valoare) <= 8;
4a. select distinct s.nume||' '||s.prenume as student 
	from studenti s 
	join note n on s.nr_matricol = n.nr_matricol 
	minus 
	select distinct s2.nume||' '||s2.prenume 
	from studenti s2 
	join note n2 on s2.nr_matricol = n2.nr_matricol 
	where n2.valoare < 7 ;
4b. select s.nume||' '||s.prenume as Student, min(n.valoare) as nota_minima 
	from studenti s 
	join note n on s.nr_matricol = n.nr_matricol 
	group by s.nume||' '||s.prenume 
	having min(n.valoare) >= 7;
5. select distinct s.nume||' '||s.prenume as Student, to_char(n.data_notare, 'Day') as Zi_din_saptamana, n.valoare as Nota 
	from studenti s 
	join note n on s.nr_matricol = n.nr_matricol 
	join cursuri c on n.id_curs = c.id_curs 
	where c.titlu_curs = 'OOP' and (n.valoare = 10 or n.valoare = 7) and trim(to_char(n.data_notare, 'Day')) = 'Tuesday';
6. select s.nume, s.prenume, n.valoare as Nota, trim(to_char(n.data_notare,'Month')) ||', '||trim(to_char(n.data_notare,'yyyy')) as Data, decode(to_char(last_day(n.data_notare),'dd'),'28','+','29','+',null) as Grea 
	from studenti s 
	natural join note n 
	where n.valoare >= 5;                                                                                             
