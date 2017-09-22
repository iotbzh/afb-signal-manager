/*
 * Copyright (C) 2015, 2016 "IoT.bzh"
 * Author "Romain Forlot" <romain.forlot@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
#pragma once

#include <memory>
#include <vector>
#include <string>

#include "source.hpp"

typedef struct clientAppCtxS
{
	std::vector<std::shared_ptr<Signal>> subscribedSignals;
	struct afb_event event;
} clientAppCtxT;

class Composer
{
private:
	CtlConfigT* ctlConfig_;

	static CtlSectionT ctlSections_[]; ///< Config Section definition (note: controls section index should match handle retrieval in)
	std::vector<SourceAPI> sourcesListV_;
	std::vector<clientAppCtxT*> subscriptions_;

	explicit Composer(const std::string& filepath);
	Composer();
	~Composer();

	CtlActionT* convert2Action(const std::string& name, json_object* action);

	int loadOneSourceAPI(json_object* sourcesJ);
	static int loadSourcesAPI(CtlSectionT* section, json_object *signalsJ);

	int loadOneSignal(json_object* signalsJ);
	static int loadSignals(CtlSectionT* section, json_object *signalsJ);

	void processOptions(const char** opts, std::shared_ptr<Signal> sig, json_object* response) const;
public:
	static Composer& instance();
	static void* createContext(void* ctx);
	static void destroyContext(void* ctx);
	static std::vector<std::string> parseURI(const std::string& uri);
	int loadConfig(const std::string& filepath);
	int loadSignals(json_object* signalsJ);

	CtlConfigT* ctlConfig();
	int initSourcesAPI();
	std::vector<std::shared_ptr<Signal>> getAllSignals();
	SourceAPI* getSourceAPI(const std::string& api);
	std::vector<std::shared_ptr<Signal>> searchSignals(const std::string& aName);
	json_object* getSignalValue(const std::string& sig, json_object* options);

	int execSignalsSubscription();

	void addSubscription(clientAppCtxT* ctx);
	void removeSubscription(clientAppCtxT* ctx);
};

struct pluginCBT
{
	void (*setSignalValue)(const char* aName, long long int timestamp, struct SignalValue value);
};

extern "C" void setSignalValueHandle(const char* aName, long long int timestamp, struct SignalValue value);
