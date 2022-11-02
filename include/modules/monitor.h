/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2021 Sadie Powell <sadie@witchery.services>
 *
 * This file is part of InspIRCd.  InspIRCd is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

namespace Monitor
{
	class APIBase;
	class API;

	class ForEachWatcherHandler
	{
	public:
	/** Method to execute for each watcher of a user.
	 * Derived classes must implement this.
	 * @param user Current watcher of the user
	 */
	virtual void Execute(LocalUser* user) = 0;
	};
}

class Monitor::APIBase
	: public DataProvider
{
 public:
	APIBase(Module* parent)
	  : DataProvider(parent, "monitor")
	{
	}

	/** Runs the provided handler for all watchers of a user
	* @param user The user to check watchers for.
	* @param handler The handler to execute for each watcher of that event.
	* @param extended_only Whether to only run the handler for watchers using the extended-notify cap.
	*/
	virtual void ForEachWatcher(User* user, ForEachWatcherHandler& handler, bool extended_only = true) = 0;
};

class Monitor::API CXX11_FINAL
	: public dynamic_reference<Monitor::APIBase>
{
 public:
	API(Module* parent)
	: dynamic_reference<Monitor::APIBase>(parent, "monitor")
	{
	}
};
