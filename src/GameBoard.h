#include <vector>

class CGameBoard {

public:
	explicit CGameBoard(void);
	virtual ~CGameBoard(void);

	t_chemin solution(void);
	void move(int mvt, unsigned int num_vehicule);

	void get_parking_occupation(int *tableau);
	bool is_move_impossible(int mvt, unsigned int num_vehicule);

public:
	std::vector<CGameBlock> vehicules;
};
