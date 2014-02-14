/*************************************************************************/
/*  sample_library.cpp                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "sample_library.h"


bool SampleLibrary::_set(const StringName& p_name, const Variant& p_value) {


	if (String(p_name).begins_with("samples/")) {

		String name=String(p_name).get_slice("/",1);
		if (p_value.get_type()==Variant::NIL)
			sample_map.erase(name);
		else {
			SampleData sd;

			if (p_value.get_type()==Variant::OBJECT)
				sd.sample=p_value;
			else if (p_value.get_type()==Variant::DICTIONARY) {

				Dictionary d = p_value;
				ERR_FAIL_COND_V(!d.has("sample"),false);
				ERR_FAIL_COND_V(!d.has("pitch"),false);
				ERR_FAIL_COND_V(!d.has("db"),false);
				sd.sample=d["sample"];
				sd.pitch_scale=d["pitch"];
				sd.db=d["db"];
			}

			sample_map[name]=sd;
		}

		return true;
	}

	return false;
}

bool SampleLibrary::_get(const StringName& p_name,Variant &r_ret) const {

	if (String(p_name).begins_with("samples/")) {

		String name=String(p_name).get_slice("/",1);
    auto iter = sample_map.find(name);
		if(iter != sample_map.end()) {
			Dictionary d;
			d["sample"]=iter->second.sample;
			d["pitch"]=iter->second.pitch_scale;
			d["db"]=iter->second.db;
			r_ret=d;
		} else {
			return false;
		}

		return true;
	}

	return false;


}

void SampleLibrary::add_sample(const StringName& p_name, const Ref<Sample>& p_sample) {

	ERR_FAIL_COND(p_sample.is_null());

	SampleData sd;
	sd.sample=p_sample;
	sample_map[p_name]=sd;
}

Ref<Sample> SampleLibrary::get_sample(const StringName& p_name) const {

  auto iter = sample_map.find(p_name);
  ERR_FAIL_COND_V(iter == sample_map.end(),Ref<Sample>());

	return iter->second.sample;
}

void SampleLibrary::remove_sample(const StringName& p_name) {

	sample_map.erase(p_name);
}

void SampleLibrary::get_sample_list(List<StringName> *p_samples) {

  for(auto& element : sample_map) {

		p_samples->push_back(element.first);
	}

}

bool SampleLibrary::has_sample(const StringName& p_name) const {

	return sample_map.find(p_name) != sample_map.end();
}

void SampleLibrary::_get_property_list(List<PropertyInfo> *p_list) const {

  for(auto& element : sample_map) {

		p_list->push_back( PropertyInfo( Variant::DICTIONARY, "samples/"+element.first,PROPERTY_HINT_RESOURCE_TYPE,"Sample",PROPERTY_USAGE_NOEDITOR ) );
	}
}

StringName SampleLibrary::get_sample_idx(int p_idx) const {

	int idx=0;
  for(auto& element : sample_map) {

		if (p_idx==idx)
			return element.first;
		idx++;
	}

	return "";
}

void SampleLibrary::sample_set_volume_db(const StringName& p_name, float p_db) {

  auto iter = sample_map.find(p_name);
	ERR_FAIL_COND(iter == sample_map.end());
	iter->second.db=p_db;

}

float SampleLibrary::sample_get_volume_db(const StringName& p_name) const{

  auto iter = sample_map.find(p_name);
	ERR_FAIL_COND_V(iter == sample_map.end(),0 );
	return iter->second.db;
}

void SampleLibrary::sample_set_pitch_scale(const StringName& p_name, float p_pitch){

  auto iter = sample_map.find(p_name);
	ERR_FAIL_COND(iter == sample_map.end());
	iter->second.pitch_scale=p_pitch;
}

float SampleLibrary::sample_get_pitch_scale(const StringName& p_name) const{

  auto iter = sample_map.find(p_name);
	ERR_FAIL_COND_V(iter == sample_map.end(),0 );
	return iter->second.pitch_scale;
}



void SampleLibrary::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("add_sample","name","sample:Sample"),&SampleLibrary::add_sample );
	ObjectTypeDB::bind_method(_MD("get_sample:Sample","name"),&SampleLibrary::get_sample );
	ObjectTypeDB::bind_method(_MD("has_sample","name"),&SampleLibrary::has_sample );
	ObjectTypeDB::bind_method(_MD("remove_sample","name"),&SampleLibrary::remove_sample );

	ObjectTypeDB::bind_method(_MD("sample_set_volume_db","name","db"),&SampleLibrary::sample_set_volume_db );
	ObjectTypeDB::bind_method(_MD("sample_get_volume_db","name"),&SampleLibrary::sample_get_volume_db );

	ObjectTypeDB::bind_method(_MD("sample_set_pitch_scale","name","pitch"),&SampleLibrary::sample_set_pitch_scale );
	ObjectTypeDB::bind_method(_MD("sample_get_pitch_scale","name"),&SampleLibrary::sample_get_pitch_scale );


}

SampleLibrary::SampleLibrary()
{
}
