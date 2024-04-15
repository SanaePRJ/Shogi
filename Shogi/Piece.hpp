#ifndef PIECE_HPP
#define PIECE_HPP


#include <utility>
#include <stdint.h>
#include <algorithm>




//�x�[�X�N���X
class Piece {
public:
	using Position = std::pair<int64_t,int64_t>;

	//���������ǂ���
	bool    IsGrowed = false;

	uint8_t ID   = UINT8_MAX; //���
	uint8_t User = UINT8_MAX; //���L��


	//�����Ă��Ȃ����ǂ���
	inline bool NotMove(Position from,Position to) const noexcept
	{
		if (from == to && from.first == 0 && from.second == 0)
			return true;

		return false;
	}

	//�ω����擾
	inline Position GetDelta(Position from,Position to) const noexcept
	{
		return {(from.first - to.first) ,(from.second - to.second)};
	}

	//��𓮂������Ƃ����Ƃ��ɌĂяo���֐�(����ȓ����ȏꍇtrue,�ُ�ȏꍇfalse)
	inline virtual bool JudgeFunc(Position from, Position to) = 0;
};




//��
class Air :public Piece {
public:
	Air()
	{
		ID   = 0;
		User = UINT8_MAX; //���L�҂Ȃ�
	}

	//�󔒂��ړ������Ă͂Ȃ�Ȃ�
	bool JudgeFunc(Position from, Position to) noexcept override 
	{
		return false;
	}
};


//����
class King :public Piece
{
public:
	King()
	{
		ID   = 1;
		User = UINT8_MAX; //���L�҂Ȃ�
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//�����Ă��Ȃ��ꍇ��false��Ԃ��B
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from,to); //�ω���

		//�S����1�}�X�ł���Γ�����B
		if (std::abs(Delta.first) <= 1 && std::abs(Delta.second) <= 1)
			return true;

		return false;
	}
};


//���̓���(���̓����͂悭����)
bool GoldGeneralMove(Piece::Position Delta) 
{
	//�c�ړ�1�ŉ��ړ��Ȃ��̏ꍇtrue
	if ((std::abs(Delta.first) == 1) && (Delta.second == 0))
		return true;

	//���ړ�1�ŏc�ړ��Ȃ��̏ꍇtrue
	if ((std::abs(Delta.second) == 1) && (Delta.first == 0))
		return true;

	//��ړ�1�ŉE1��1�ł����true
	if ((Delta.first == 1) && (std::abs(Delta.second) == 1))
		return true;

	return false;
}


//����
class GoldGeneral :public Piece
{
public:
	GoldGeneral()
	{
		ID   = 2;
		User = UINT8_MAX; //���L�҂Ȃ�
	}

	bool JudgeFunc(Position from, Position to) noexcept override
	{
		//�����Ă��Ȃ��ꍇ��false��Ԃ��B
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //�ω���

		return GoldGeneralMove(Delta);
	}
};


//�⏫->����
class SilverGeneral :public Piece
{
public:
	SilverGeneral()
	{
		ID   = 3;
		User = UINT8_MAX; //���L�҂Ȃ�
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//�����Ă��Ȃ��ꍇ��false��Ԃ��B
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //�ω���

		//�����Ă����ꍇ���Ɠ�������
		if (this->IsGrowed)
			return GoldGeneralMove(Delta);

		//�΂�1�ł����true
		if ((std::abs(Delta.first) == 1) && (std::abs(Delta.second) == 1))
			return true;

		//��1�ł����true
		if (Delta.first == 1 && Delta.second == 0)
			return true;

		return false;
	}
};


//�j�n->���j
class Knight :public Piece
{
public:
	Knight()
	{
		ID = 4;
		User = UINT8_MAX; //���L�҂Ȃ�
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//�����Ă��Ȃ��ꍇ��false��Ԃ��B
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //�ω���

		//�����Ă����ꍇ���Ɠ�������
		if (this->IsGrowed)
			return GoldGeneralMove(Delta);

		//���E1�̏�2�ł����true
		if ((std::abs(Delta.second) == 1) && (Delta.first == 2))
			return true;

		return false;
	}
};


//����->����
class Lance :public Piece
{
public:
	Lance()
	{
		ID = 5;
		User = UINT8_MAX; //���L�҂Ȃ�
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//�����Ă��Ȃ��ꍇ��false��Ԃ��B
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //�ω���

		//�����Ă����ꍇ���Ɠ�������
		if (this->IsGrowed)
			return GoldGeneralMove(Delta);

		//���ړ��Ȃ��ŏ�ֈړ�����ꍇ
		if (Delta.second == 0 && Delta.first > 0)
			return true;

		return false;
	}
};


//���->����
class Rook :public Piece
{
public:
	Rook()
	{
		ID = 5;
		User = UINT8_MAX; //���L�҂Ȃ�
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//�����Ă��Ȃ��ꍇ��false��Ԃ��B
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //�ω���

		//�c�ړ��݂̂������ꍇ
		if (std::abs(Delta.first) != 0 && Delta.second == 0)
			return true;
		//���ړ��݂̂������ꍇ
		if (std::abs(Delta.second) != 0 && Delta.first == 0)
			return true;

		//�����Ă���ꍇ�΂�1�ł���Έړ��ł���B
		if (this->IsGrowed && std::abs(Delta.first) == 1 && std::abs(Delta.second) == 1)
			return true;

		return false;
	}
};


//�p�s->���n
class Bishop :public Piece
{
public:
	Bishop()
	{
		ID = 6;
		User = UINT8_MAX; //���L�҂Ȃ�
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//�����Ă��Ȃ��ꍇ��false��Ԃ��B
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //�ω���

		//�΂߈ړ��ł����true
		if (std::abs(Delta.first) == std::abs(Delta.second))
			return true;

		//�������ꍇ�c1�E��1�ł����true
		if (this->IsGrowed && (std::abs(Delta.first) == 1 || std::abs(Delta.second) == 1))
			return true;

		return false;
	}
};


//����->�Ƌ�
class Pawn :public Piece
{
public:
	Pawn()
	{
		ID   = 7;
		User = UINT8_MAX; //���L�҂Ȃ�
	}

	bool JudgeFunc(Position from, Position to) override
	{
		//�����Ă��Ȃ��ꍇ��false��Ԃ��B
		if (NotMove(from, to))
			return false;

		Position Delta = this->GetDelta(from, to); //�ω���

		//�����Ă����ꍇ���Ɠ�������
		if (this->IsGrowed)
			return GoldGeneralMove(Delta);

		//��1�ɂ̂ݓ���\
		if (Delta.first == 1 && Delta.second == 0)
			return true;

		return false;
	}
};




#endif