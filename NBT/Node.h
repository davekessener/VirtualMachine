#ifndef NODE_H
#define NODE_H

#ifdef NODE_MAIN
#endif

class Node
{
	public:
		typedef std::shared_ptr<Node> Node_ptr_t;

		virtual inline ~Node( ) { }
		virtual std::vector<Node_ptr_t> getChildren( ) = 0;
		virtual std::string getValue( ) = 0;
		virtual void insert(Node_ptr_t) = 0;
		virtual void remove(Node_ptr_t) = 0;
		virtual inline bool hasChildren( ) { return !getChildren().empty(); }
		inline bool isOpen( ) { return hasChildren() && isOpened; }
		inline void open( ) { isOpened = true; }
		inline void close( ) { isOpened = false; }
		inline void toggle( ) { isOpened = !isOpened; }
	private:
		bool isOpened;
};

#endif

