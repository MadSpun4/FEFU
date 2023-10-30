Описание предметной области:
Школа, в которой меняется директор, завуч, учителя. Меняется расписание завучем. Школьники получают вопрос от учителя и отвечают на него.

Анализ сущностей ПО:
Школа: может удалять и добавлять нового директора(+ addDirector():str, + deleteDirector():str), принимать и удалять новых школьников(+ addStudent(students_list: students_list):students_list, + deletStudent(students_list: students_list):students_list).
Директор: ставит на пост и снимает с него завуча(+ addZavuch(Завуч.name: str):str, deleteZavuch(Завуч.name: str):str).
Завуч: меняет расписание(+ chengeSchedule(Расписание): Расписание), принимает на работу и увольняет учителей(+ addTeacher(teacher_list: teacher_ list; Учитель.name: str): teacher_list, + deleteTeacher(teacher_list: teacher_list): teacher_list).
Учитель: хранится в списке учителей школы(teacher_list:teacher_list), задает вопросы ученикам(+ askStudent(Ученик.name): bool).
Ученик: хранится в списке учеников школы(students_list:students_list), отвечает учителю(+ answerToTeacher(): bool).
Расписание: меняется завучем.

