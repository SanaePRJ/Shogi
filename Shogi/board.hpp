#ifndef SANAE_BOARD_HPP
#define SANAE_BOARD_HPP


#include <vector>
#include <functional>
#include <stdexcept>




template<typename PieceType>
class board {
private:
	template<typename Type> 
	using Matrix   = std::vector<std::vector<Type>>;
	using Position = std::pair  <size_t, size_t>;

	Matrix<PieceType> m_board;

public:
	board() = default;
	board(const std::initializer_list<std::initializer_list<PieceType>>& Data)
	{
		std::copy(Data.begin(),Data.end(),std::back_inserter(m_board));
	}


	board& operator =(const std::initializer_list<std::initializer_list<PieceType>>& Data)
	{
		//m_board�폜
		m_board.erase(m_board.begin(),m_board.end());
		//�R�s�[
		std::copy(Data.begin(), Data.end(), std::back_inserter(m_board));

		return *this;
	}


	//board�̃T�C�Y��ύX
	board& SetSize(size_t a_row,size_t a_column)
	{
		m_board.resize(a_row,std::vector<PieceType>(a_row));

		return *this;
	}


	//�Q��
	std::vector<PieceType>& operator[](size_t a_row) 
	{
		return m_board[a_row];
	}


	//std::vector<PieceType>��board�ɓ����B
	board& Set(const std::vector<PieceType>& a_data,size_t a_row,bool Assign_Left=true)
	{
		//�{�[�h�T�C�Y�O�܂�set���悤�Ƃ����ꍇ
		if(static_cast<std::vector<PieceType>>(m_board[a_row]).size() < a_data.size())
			throw std::runtime_error("Set����T�C�Y���{�[�h�T�C�Y�𒴂��Ă��܂��B");

		//�{�[�h�̃C�e���[�^
		auto board_itr = m_board[a_row].begin();
		//Set���鏇�Ԃ������炶��Ȃ��ꍇ(a_data.end()-1)�̃C�e���[�^��n���B
		auto data_itr  = Assign_Left ? (a_data.begin()) : (--a_data.end());

		for (size_t column = 0; column < a_data.size();column++) 
		{
			//m_board��data�����Ă���
			*board_itr = *data_itr;

			//m_board�̃C�e���[�^��i�߂�B
			board_itr++;

			//a_data�̃C�e���[�^��i�߂�or�߂��B
			if (Assign_Left) 
				data_itr++;//�C�e���[�^��i�߂�B
			else             
				data_itr--;//�C�e���[�^��߂��B
		}

		return *this;
	}


	//���v���Ƀ�[rad]��]������B
	board& RotateClockWise() 
	{
		const size_t row    = m_board.size();   //�s��
		const size_t column = m_board[0].size();//��(0����Ƃ���)

		//��]�������f�[�^���i�[
		Matrix<PieceType> Rotated(column,std::vector<PieceType>(row));

		//���ׂẴf�[�^���i�[
		for (size_t r = 0; r < row;r++)
			for (size_t c = 0;c < column;c++)
				Rotated[c][row - r - 1] = m_board[r][c];

		//���L�����ړ�
		m_board = std::move(Rotated);

		return *this;
	}


	//�����v���Ƀ�[rad]��]������B
	board& RotateCounterClockWise()
	{
		const size_t row    = m_board.size();   //�s��
		const size_t column = m_board[0].size();//��(0����Ƃ���)

		//��]�������f�[�^���i�[
		Matrix<PieceType> Rotated(column, std::vector<PieceType>(row));

		//���ׂẴf�[�^���i�[
		for (size_t r = 0; r < row; r++)
			for (size_t c = 0; c < column; c++)
				Rotated[column - c - 1][r] = m_board[r][c];

		//���L�����ړ�
		m_board = std::move(Rotated);

		return *this;
	}


	//jfunc(���������������ǂ����𔻒肷��֐�),�ړ������ۂɌ��̏ꏊ�ɖ��߂�^�C�v������,�ړ���̋��Ԃ��B
	PieceType MovePiece(const Position& from,const Position& to,std::function<bool(Position,Position)> jfunc,const PieceType& FillPiece) {
		//�{�[�h�T�C�Y�𒴂���from�̃|�C���g���w�肳�ꂽ�ꍇ
		if (from.first > m_board.size() || from.second > m_board[0].size())
			throw std::invalid_argument("�{�[�h�O��from�Ɏw�肳��܂����B");

		//�{�[�h�T�C�Y�𒴂���to�̃|�C���g���w�肳�ꂽ�ꍇ
		if (to.first > m_board.size() || to.second > m_board[0].size())
			throw std::invalid_argument("�{�[�h�O��to�Ɏw�肳��܂����B");
		
		if (jfunc(from, to)) {
			//�z�u��̋��ۑ�
			PieceType to_piece = m_board[to.first][to.second];

			//from����to�֋���ړ�
			m_board[to.first][to.second] = m_board[from.first][from.second];

			//�ړ�����fill����B
			m_board[from.first][from.second] = FillPiece;

			return to_piece;
		}

		return FillPiece;
	}


	auto begin() 
	{
		return m_board.begin();
	}
	auto end()
	{
		return m_board.end();
	}
};




#endif