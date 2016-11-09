/*!
 * \file   ThreadPool.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   19 juin 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include <memory>
#include <stdexcept>
#include"TFEL/System/ThreadPool.hxx"

namespace tfel{

  namespace system{
   
    ThreadPool::ThreadPool(const size_t n)
    {
      for(size_t i = 0;i<n;++i){
	auto f = [this]{
	  for(;;){
	    std::function<void()> task;
	    {
	      std::unique_lock<std::mutex> lock(this->queue_mutex);
	      this->condition.wait(lock,[this]{
		  return this->stop || !this->tasks.empty();
		});
	      if(this->stop && this->tasks.empty()){
		return;
	      }
	      task = std::move(this->tasks.front());
	      this->tasks.pop();
	    }
	    task();
	  }
	};
	this->workers.emplace_back(f);
      }
    }    

    ThreadPool::~ThreadPool()
    {
      {
	std::unique_lock<std::mutex> lock(queue_mutex);
	this->stop = true;
      }
      condition.notify_all();
      // the destructor joins all threads
      for(auto &w: this->workers){
	w.join();
      }
    }

  } // end of namespace system

} // end of namespace tfel
