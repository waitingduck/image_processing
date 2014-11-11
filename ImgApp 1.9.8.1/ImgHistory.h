#include <atlimage.h>

class ImageHistoryEntity
{
public:
	CImage *m_pImage;
	ImageHistoryEntity *m_pLast;
	ImageHistoryEntity *m_pNext;

public:
	ImageHistoryEntity();
	int AddHistoryEntity();
	int DeleteHistoryEntity();
};


class ImageHistory
{
public:
	ImageHistoryEntity *m_pInstantHistory;

public:
	ImageHistory();
	int Add();
	int GetCount();
};
