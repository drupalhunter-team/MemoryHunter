typedef struct {
  	char first[32];
  	char last[32];
  	int  age;
} PersonType;

typedef struct {
  	PersonType basicInfo;
  	char       stuNumber[12];
  	float      gpa;
} StudentType;

typedef struct {
	PersonType	basicInfo;
	char		jobTitle[32];
	int		yearsExperience;
} EmployeeType;

typedef struct {
	PersonType	basicInfo;
	char		sport[32];
	char		position[32];
	int		points;
} PlayerType;

typedef struct {
	PersonType	basicInfo;
	EmployeeType	jobInfo;
	char		rank[32];
} MilitaryType;

typedef struct {
	PersonType 	basicInfo;
	EmployeeType	jobInfo;
	char		prefix[8];
	char 		courseTaught;
} InstructorType;

typedef struct {
	char 	provider[32];
	char	OS[32];
	int 	age;
	char	model[32];
} CellType;
