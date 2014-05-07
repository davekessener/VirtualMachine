#ifndef SIM_CONNECTOR_H
#define SIM_CONNECTOR_H

#include <memory>
#include "Sim.h"
#include "Conductable.h"
#include "Wire.h"

namespace sim
{
	class Connector : public Conductable
	{
		public:
		typedef std::shared_ptr<Connector> Connector_ptr;

		public:
			Connector( );
			void setInput(Wire::Wire_ptr);
			V_t out( ) const;
			void in(const V_t&);
		private:
			Wire::Wire_ptr node_;
			V_t v_;
	};

// # ===========================================================================
//
//	class Connector : public Conductable
//	{
//		public:
//		typedef std::shared_ptr<Connector> Connector_ptr;
//
//		public:
//			Connector(Wire::Wire_ptr);
//			void connect(Connector_ptr);
//		protected:
//			Connector_ptr pin_;
//			Wire::Wire_ptr node_;
//			friend class ConnectorOutput;
//	};
//
//	class ConnectorInput : public Connector
//	{
//		public:
//			ConnectorInput(Wire::Wire_ptr);
//			V_t out( ) const;
//			void in(const V_t&);
//		protected:
//		private:
//			V_t v_;
//	};
//
//	class ConnectorOutput : public Connector
//	{
//		public:
//			ConnectorOutput(Wire::Wire_ptr);
//			V_t out( ) const;
//		protected:
//		private:
//	};
}

#endif

