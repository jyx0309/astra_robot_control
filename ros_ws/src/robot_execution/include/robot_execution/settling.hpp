#pragma once
#include <cmath>
#include <deque>
#include <vector>
namespace robot_execution {
// Controller state=1 also occurs while holding position. Determine settling
// from actual samples, never from the enum alone.
class Settling {
 struct Sample {double time;std::vector<double> position,velocity;};
 std::deque<Sample> samples_;
public:
 void update(double time,const std::vector<double>& p,const std::vector<double>& v) {
  bool valid=!p.empty();for(double x:p)valid=valid&&std::isfinite(x);
  for(double x:v)valid=valid&&std::isfinite(x)&&std::abs(x)<0.1;
  if(!v.empty()&&v.size()!=p.size())valid=false;
  if(!valid){samples_.clear();return;}
  if(!samples_.empty()&&(time<=samples_.back().time||p.size()!=samples_.back().position.size()))samples_.clear();
  for(const auto& sample:samples_)for(size_t i=0;i<p.size();++i)
   if(std::abs(sample.position[i]-p[i])>0.002){samples_.clear();samples_.push_back({time,p,v});return;}
  samples_.push_back({time,p,v});
  while(samples_.size()>2&&time-samples_[1].time>=0.25)samples_.pop_front();
 }
 bool stationary(double time)const {
  if(samples_.size()<3||samples_.back().time-samples_.front().time<0.25||time-samples_.back().time>=0.5)return false;
  std::vector<double> sums(samples_.back().position.size(),0.0);
  size_t count=0;
  for(const auto& s:samples_)if(s.velocity.size()==sums.size()) {
   ++count;for(size_t i=0;i<sums.size();++i)sums[i]+=s.velocity[i];
  }
  if(count)for(double sum:sums)if(std::abs(sum/count)>0.02)return false;
  return true;
 }
};
inline bool position_controller_state(int state){return state==0||state==1;}
}
