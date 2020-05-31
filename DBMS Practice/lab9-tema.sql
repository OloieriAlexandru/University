
DROP INDEX note_id_student;
/

DROP INDEX note_id_curs;
/

CREATE INDEX note_id_student
ON note(id_student);
/

CREATE INDEX note_id_curs
ON note(id_curs);
/
