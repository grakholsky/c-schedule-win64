/* 

typedef struct {
	size_t id;
	char* FullName;
	char* LastName;
	char* MiddleName;
	char* AcademicRank;
	char* Role;
	char* About;
	char* PhotoLink;
	size_t id_cafedras;
} Teacher;

typedef struct {
	size_t id;
	char* FullName;
	char* LastName;
	char* MiddleName;
	char* NumberNote;
	char* NumberCard;
	char* StudyMode;
	size_t id_groups;
} Student;

typedef struct {
	size_t id;
	char* title;
	size_t id_cafedras;
} Group;

typedef struct {
	size_t id;
	size_t id_lesson;
	size_t id_days;
	size_t id_groups;
	size_t id_teacher;
	size_t id_campuses;
	size_t id_type;
	size_t id_room;
} Task;

typedef struct {
	size_t id;
	char* title;
} Faculty;

typedef struct {
	size_t id;
	char* title;
	size_t id_faculties;
};

*/