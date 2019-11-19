#include <iostream>
#include <map>
#include <iomanip>
#include <vector>
#include <string>

class TextTable {

    public:
    enum class Alignment { LEFT, RIGHT }; 
    typedef std::vector< std::string > Row;//数据表每一行的数据
    TextTable( char horizontal = '-', char vertical = '|', char corner = '+' ) :
	//初始化水平、垂直、角落的字符
        _horizontal( horizontal ),
        _vertical( vertical ),
        _corner( corner )
    {}

    void setAlignment( unsigned i, Alignment alignment ) {
        _alignment[ i ] = alignment;
    }

    Alignment alignment( unsigned i ) const { return _alignment[ i ]; }

    char vertical() const { return _vertical; } 

    char horizontal() const { return _horizontal; }

    void add( std::string const & content ) {//插入字符串
        _current.push_back( content );
    }

    void endOfRow()    {
        _rows.push_back( _current );//更新当前行所有字符串
        _current.assign( 0, "" );//将当前串置为空，方便下一次重新插入字符串
    }

    template <typename Iterator>
    void addRow( Iterator begin, Iterator end ){//按顺序插入数据
        for( auto i = begin; i != end; ++i ) {
           add( * i ); 
        }
        endOfRow();
    }

    template <typename Container>
    void addRow( Container const & container )    {
        addRow( container.begin(), container.end() );
    }

    std::vector< Row > const & rows() const {
        return _rows;
    }

    void setup() const //初始化制表
    {
        determineWidths();
        setupAlignment();
    }

    std::string ruler() const{//每一行分隔符的叠加
        std::string result;
        result += _corner;
	//按照找到的宽度叠加水平分隔符和角落分隔符
        for( auto width = _width.begin(); width != _width.end(); ++ width ) { 
            result += repeat( * width, _horizontal );
            result += _corner;
        }

        return result;
    }

    int width( unsigned i ) const
    { return _width[ i ]; }

    private:
    char _horizontal;//水平方向分隔符
    char _vertical;//垂直方向分隔符
    char _corner;//角标
    Row _current;//当前每一行的字符串
    std::vector< Row > _rows; //所有字符的叠加
    std::vector< unsigned > mutable _width;//每一个区间的宽度
    std::map< unsigned, Alignment > mutable _alignment;

    static std::string repeat( unsigned times, char c ){//便于叠加水平分隔符
        std::string result;
        while(times--){
            result += c;
	}

        return result;
    }

    unsigned columns() const {//找列宽
        return _rows[ 0 ].size();
    }

    void determineWidths() const {//根据字符长度算每一列宽度
        _width.assign( columns(), 0 );
        for ( auto rowIterator = _rows.begin(); rowIterator != _rows.end(); ++ rowIterator ) {
            Row const & row = * rowIterator;
            for ( unsigned i = 0; i < row.size(); ++i ) {
                _width[ i ] = _width[ i ] > row[ i ].size() ? _width[ i ] : row[ i ].size();
            }
        }
    }

    void setupAlignment() const {//初始化
        for ( unsigned i = 0; i < columns(); ++i ) {
            if ( _alignment.find( i ) == _alignment.end() ) {
                _alignment[ i ] = Alignment::LEFT;
            }
        }
    }
};


//重载运算符
std::ostream & operator<<( std::ostream & stream, TextTable const & table ){
    table.setup();
    stream << table.ruler() << "\n";
    for ( auto rowIterator = table.rows().begin(); rowIterator != table.rows().end(); ++ rowIterator ) {
	//输出制表内容
        TextTable::Row const & row = * rowIterator;
        stream << table.vertical();
        for ( unsigned i = 0; i < row.size(); ++i ) {
            auto alignment = table.alignment( i ) == TextTable::Alignment::LEFT ? std::left : std::right;
            stream << std::setw( table.width( i ) ) << alignment << row[ i ];
            stream << table.vertical();
        }
        stream << "\n";
        stream << table.ruler() << "\n";
    }

    return stream;
}
