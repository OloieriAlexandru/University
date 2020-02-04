-- ex1
select Studenti.nume||' '||Studenti.prenume as Student, Note.valoare as Nota, Profesori.nume||' '|| Profesori.prenume as Profesor 
	from studenti join note on Studenti.nr_matricol = note.nr_matricol 
	join Cursuri on Cursuri.id_curs = Note.id_curs 
	join Didactic on Didactic.id_curs = Cursuri.id_curs 
	join Profesori on Profesori.id_prof = Didactic.id_prof;
-- ex2
select s.nume||' '||s.prenume as Student, note.valoare as Nota, cursuri.titlu_curs as Curs from studenti s 
	join note on s.nr_matricol = note.nr_matricol 
	join cursuri on cursuri.id_curs = note.id_curs 
	where cursuri.titlu_curs = 'BD' and note.valoare = 10; 
-- ex3
select nume, prenume, titlu_curs from profesori
	join didactic on profesori.id_prof = didactic.id_prof
	join cursuri on didactic.id_curs = cursuri.id_curs;
-- ex4
select nume, prenume, titlu_curs from profesori
	left outer join didactic on profesori.id_prof = didactic.id_prof
	left outer join cursuri on didactic.id_curs = cursuri.id_curs;
-- ex5
select c.titlu_curs 
	from cursuri c
	minus
	select c.titlu_curs 
	from cursuri c natural join didactic d;
-- ex5rau
select nume, prenume, titlu_curs from profesori
	join didactic on profesori.id_prof = didactic.id_prof
	right outer join cursuri on didactic.id_curs = cursuri.id_curs;
-- ex6a
select nume, prenume, titlu_curs from profesori
	full outer join didactic on profesori.id_prof = didactic.id_prof
	full outer join cursuri on didactic.id_curs = cursuri.id_curs;
-- ex6b
select c.titlu_curs, nume, prenume 
	from profesori p 
	full outer join didactic d on d.id_prof = p.id_prof 
	full outer join cursuri c ON c.id_curs = d.id_curs 
	minus 
	select c.titlu_curs, nume, prenume 
	from profesori 
	natural join didactic 
	natural join cursuri c;                                       
-- ex7
select s1.nume, s2.nume, to_char(s1.data_nastere, 'Day') as "Zi a saptamanii" from studenti s1
	join studenti s2 on to_char(s1.data_nastere, 'Day') = to_char(s2.data_nastere, 'Day')
	where s1.nr_matricol != s2.nr_matricol and s1.nume < s2.nume
	order by 3, 1;                                            
-- ex8
select s1.nume||' '||s1.prenume||' a luat nota mai mare decat '||s2.nume||' '||s2.prenume||' la materia '||cursuri.titlu_curs from studenti s1
	cross join studenti s2 join note n1 on s1.nr_matricol = n1.nr_matricol 
	join note n2 on s2.nr_matricol = n2.nr_matricol 
	join cursuri on n1.id_curs = cursuri.id_curs 
	where n1.valoare > n2.valoare and n1.id_curs = n2.id_curs;
	