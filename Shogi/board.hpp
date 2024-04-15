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
		//m_board削除
		m_board.erase(m_board.begin(),m_board.end());
		//コピー
		std::copy(Data.begin(), Data.end(), std::back_inserter(m_board));

		return *this;
	}


	//boardのサイズを変更
	board& SetSize(size_t a_row,size_t a_column)
	{
		m_board.resize(a_row,std::vector<PieceType>(a_row));

		return *this;
	}


	//参照
	std::vector<PieceType>& operator[](size_t a_row) 
	{
		return m_board[a_row];
	}


	//std::vector<PieceType>をboardに入れる。
	board& Set(const std::vector<PieceType>& a_data,size_t a_row,bool Assign_Left=true)
	{
		//ボードサイズ外までsetしようとした場合
		if(static_cast<std::vector<PieceType>>(m_board[a_row]).size() < a_data.size())
			throw std::runtime_error("Setするサイズがボードサイズを超えています。");

		//ボードのイテレータ
		auto board_itr = m_board[a_row].begin();
		//Setする順番が左からじゃない場合(a_data.end()-1)のイテレータを渡す。
		auto data_itr  = Assign_Left ? (a_data.begin()) : (--a_data.end());

		for (size_t column = 0; column < a_data.size();column++) 
		{
			//m_boardにdataを入れていく
			*board_itr = *data_itr;

			//m_boardのイテレータを進める。
			board_itr++;

			//a_dataのイテレータを進めるor戻す。
			if (Assign_Left) 
				data_itr++;//イテレータを進める。
			else             
				data_itr--;//イテレータを戻す。
		}

		return *this;
	}


	//時計回りにπ[rad]回転させる。
	board& RotateClockWise() 
	{
		const size_t row    = m_board.size();   //行数
		const size_t column = m_board[0].size();//列数(0を基準として)

		//回転させたデータを格納
		Matrix<PieceType> Rotated(column,std::vector<PieceType>(row));

		//すべてのデータを格納
		for (size_t r = 0; r < row;r++)
			for (size_t c = 0;c < column;c++)
				Rotated[c][row - r - 1] = m_board[r][c];

		//所有権を移動
		m_board = std::move(Rotated);

		return *this;
	}


	//反時計回りにπ[rad]回転させる。
	board& RotateCounterClockWise()
	{
		const size_t row    = m_board.size();   //行数
		const size_t column = m_board[0].size();//列数(0を基準として)

		//回転させたデータを格納
		Matrix<PieceType> Rotated(column, std::vector<PieceType>(row));

		//すべてのデータを格納
		for (size_t r = 0; r < row; r++)
			for (size_t c = 0; c < column; c++)
				Rotated[column - c - 1][r] = m_board[r][c];

		//所有権を移動
		m_board = std::move(Rotated);

		return *this;
	}


	//jfunc(動きが正しいかどうかを判定する関数),移動した際に元の場所に埋めるタイプを入れる,移動先の駒を返す。
	PieceType MovePiece(const Position& from,const Position& to,std::function<bool(Position,Position)> jfunc,const PieceType& FillPiece) {
		//ボードサイズを超えてfromのポイントが指定された場合
		if (from.first > m_board.size() || from.second > m_board[0].size())
			throw std::invalid_argument("ボード外がfromに指定されました。");

		//ボードサイズを超えてtoのポイントが指定された場合
		if (to.first > m_board.size() || to.second > m_board[0].size())
			throw std::invalid_argument("ボード外がtoに指定されました。");
		
		if (jfunc(from, to)) {
			//配置先の駒を保存
			PieceType to_piece = m_board[to.first][to.second];

			//fromからtoへ駒を移動
			m_board[to.first][to.second] = m_board[from.first][from.second];

			//移動元をfillする。
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