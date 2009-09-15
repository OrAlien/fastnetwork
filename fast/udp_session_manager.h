#pragma once
#include "endpoint.h"
#include "period_timer.h"

namespace fastnet {

	class io_session;

	namespace udp {
		using namespace std;
		using namespace boost;
		using namespace boost::asio;
		using namespace fastnet::utility;

		class udp_session;

		class udp_session_manager : noncopyable
		{
		private:
			typedef map<endpoint, shared_ptr<io_session>>	session_map;

		public:
			udp_session_manager( io_service & ios )
				: timer_(ios)
			{
				timer_.start( 1000, ::boost::bind( &udp_session_manager::on_timer, this, _1 ) );
			}
			~udp_session_manager(void){
				timer_.stop();
			}

		public:
			void on_timer( const system::error_code & err ) {
				if ( !err )
				{
					check_session_idle();
				}
			}

			shared_ptr<io_session> new_session( shared_ptr<ip::udp::socket> socket, endpoint remote );

			void close_session( shared_ptr<io_session> session );

		private:
			void check_session_idle();

		private:
			period_timer	timer_;
			session_map		sessions_;
		};
	}
}