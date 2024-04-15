#ifndef PIECE_HPP
#define PIECE_HPP


#include <utility>
#include <stdint.h>
#include <algorithm>




//ベースクラス
class Piece {
public:
	using Position = std::pair<int64_t,int64_t>;

	//成ったかどうか
	bool    IsGrowed = false;

	uint8_t ID   = UINT8_MAX; //駒識別
	uint8_t User = UINT8_MAX; //所有者


	//動いていないかどうか
	inline bool NotMove(Position from,Position to) const noexcept
	{
		if (from == to && from.first == 0 && from.second == 0)
			return true;

		return false;
	}

	//変化を取得
	inline Position GetDelta(Position from,Position to) const noexcept
	{
		return {(from.first - to.first) ,(from.second - to.second)};
	}

	//駒を動かそうとしたときに呼び出す関数(正常な動きな場合true,異常な場合false)
	inline virtual bool JudgeFunc(Position from, Position to) = 0;
};




//空白
class Air :public Piece {
public:
	Air()
	{
		ID   = 0;
		User = UINT8_MAX; //所有者なし
	}

	//空白を移動させてはならない
	bool JudgeFunc(Position from, Position to) noexcept override 
	{
		return false;
	}
};


//王将
class King :public Piece
{
public:
	King()
	{
		ID   = 1;
		User = UINT8_MAX; //所有者なし
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//動いていない場合はfalseを返す。
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from,to); //変化量

		//全方位1マスであれば動ける。
		if (std::abs(Delta.first) <= 1 && std::abs(Delta.second) <= 1)
			return true;

		return false;
	}
};


//金の動き(この動きはよくする)
bool GoldGeneralMove(Piece::Position Delta) 
{
	//縦移動1で横移動なしの場合true
	if ((std::abs(Delta.first) == 1) && (Delta.second == 0))
		return true;

	//横移動1で縦移動なしの場合true
	if ((std::abs(Delta.second) == 1) && (Delta.first == 0))
		return true;

	//上移動1で右1左1であればtrue
	if ((Delta.first == 1) && (std::abs(Delta.second) == 1))
		return true;

	return false;
}


//金将
class GoldGeneral :public Piece
{
public:
	GoldGeneral()
	{
		ID   = 2;
		User = UINT8_MAX; //所有者なし
	}

	bool JudgeFunc(Position from, Position to) noexcept override
	{
		//動いていない場合はfalseを返す。
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //変化量

		return GoldGeneralMove(Delta);
	}
};


//銀将->成銀
class SilverGeneral :public Piece
{
public:
	SilverGeneral()
	{
		ID   = 3;
		User = UINT8_MAX; //所有者なし
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//動いていない場合はfalseを返す。
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //変化量

		//成っていた場合金と同じ動き
		if (this->IsGrowed)
			return GoldGeneralMove(Delta);

		//斜め1であればtrue
		if ((std::abs(Delta.first) == 1) && (std::abs(Delta.second) == 1))
			return true;

		//上1であればtrue
		if (Delta.first == 1 && Delta.second == 0)
			return true;

		return false;
	}
};


//桂馬->成桂
class Knight :public Piece
{
public:
	Knight()
	{
		ID = 4;
		User = UINT8_MAX; //所有者なし
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//動いていない場合はfalseを返す。
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //変化量

		//成っていた場合金と同じ動き
		if (this->IsGrowed)
			return GoldGeneralMove(Delta);

		//左右1の上2であればtrue
		if ((std::abs(Delta.second) == 1) && (Delta.first == 2))
			return true;

		return false;
	}
};


//香車->成香
class Lance :public Piece
{
public:
	Lance()
	{
		ID = 5;
		User = UINT8_MAX; //所有者なし
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//動いていない場合はfalseを返す。
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //変化量

		//成っていた場合金と同じ動き
		if (this->IsGrowed)
			return GoldGeneralMove(Delta);

		//横移動なしで上へ移動する場合
		if (Delta.second == 0 && Delta.first > 0)
			return true;

		return false;
	}
};


//飛車->龍王
class Rook :public Piece
{
public:
	Rook()
	{
		ID = 5;
		User = UINT8_MAX; //所有者なし
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//動いていない場合はfalseを返す。
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //変化量

		//縦移動のみだった場合
		if (std::abs(Delta.first) != 0 && Delta.second == 0)
			return true;
		//横移動のみだった場合
		if (std::abs(Delta.second) != 0 && Delta.first == 0)
			return true;

		//成っている場合斜め1であれば移動できる。
		if (this->IsGrowed && std::abs(Delta.first) == 1 && std::abs(Delta.second) == 1)
			return true;

		return false;
	}
};


//角行->竜馬
class Bishop :public Piece
{
public:
	Bishop()
	{
		ID = 6;
		User = UINT8_MAX; //所有者なし
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//動いていない場合はfalseを返す。
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //変化量

		//斜め移動であればtrue
		if (std::abs(Delta.first) == std::abs(Delta.second))
			return true;

		//成った場合縦1・横1であればtrue
		if (this->IsGrowed && (std::abs(Delta.first) == 1 || std::abs(Delta.second) == 1))
			return true;

		return false;
	}
};


//歩兵->と金
class Pawn :public Piece
{
public:
	Pawn()
	{
		ID   = 7;
		User = UINT8_MAX; //所有者なし
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//動いていない場合はfalseを返す。
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //変化量

		//成っていた場合金と同じ動き
		if (this->IsGrowed)
			return GoldGeneralMove(Delta);

		//上1にのみ動作可能
		if (Delta.first == 1 && Delta.second == 0)
			return true;

		return false;
	}
};




#endif