#pragma once

#include "frame_producer.h"

#include <common/memory/safe_ptr.h>
#include <common/utility/printer.h>

#include <boost/noncopyable.hpp>
#include <boost/thread/future.hpp>

#include <functional>

#include <boost/signals2.hpp>

namespace caspar { namespace core {

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	frame_producer_device
///
/// \brief
/// 		
///                |**********| <-   empty frame   <- |***********| <-   frame format  <- |**********|
///   PROTOCOL ->  | PRODUCER |                       |   MIXER	  |                       | CONSUMER |  -> DISPLAY DEVICE
///                |**********| -> rendered frames -> |***********| -> formatted frame -> |**********|
///   
////////////////////////////////////////////////////////////////////////////////////////////////////

class frame_producer_device : boost::noncopyable
{
public:
	typedef boost::signals2::signal<void(const std::vector<safe_ptr<basic_frame>>&)> output_t;
	 
	boost::signals2::connection connect(const output_t::slot_type& subscriber);

	explicit frame_producer_device(const printer& parent_printer, const safe_ptr<frame_factory>& factory);
	frame_producer_device(frame_producer_device&& other);
	void swap(frame_producer_device& other);
		
	void load(int index, const safe_ptr<frame_producer>& producer, bool play_on_load = false, bool preview = false);
	void pause(int index);
	void play(int index);
	void stop(int index);
	void clear(int index);
	void clear();	
	void swap_layer(int index, size_t other_index);
	void swap_layer(int index, size_t other_index, frame_producer_device& other);
	boost::unique_future<safe_ptr<frame_producer>> foreground(size_t index) const;

private:
	struct implementation;
	safe_ptr<implementation> impl_;
};

}}