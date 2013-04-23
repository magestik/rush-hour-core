#include <vector>

class CGameBoard {

public:
	explicit CGameBoard(void);
	virtual ~CGameBoard(void);

	t_chemin solution(void);
	//bool move(void);

public:
	std::vector<CGameBlock *> vehicules;
};
