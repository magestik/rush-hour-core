class CGameBlock {

public:

	enum EAxis {
		e_axis_vertical,
		e_axis_horizontal
	};

	explicit CGameBlock(t_position pos, short size, EAxis axis);
	virtual ~CGameBlock(void);

public:
	t_position position;
	short size;
	EAxis axis; // horizontal = 0 ; vertical = 1
};
