/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2013, 2017, 2019-2020 Sadie Powell <sadie@witchery.services>
 *   Copyright (C) 2012, 2018-2019 Robby <robby@chatbelgie.be>
 *   Copyright (C) 2009 Uli Schlachter <psychon@inspircd.org>
 *   Copyright (C) 2009 Daniel De Graaf <danieldg@inspircd.org>
 *   Copyright (C) 2008, 2010 Craig Edwards <brain@inspircd.org>
 *   Copyright (C) 2008 Robin Burchell <robin+git@viroteck.net>
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


#include "inspircd.h"
#include "modules/ctctags.h"
#include "modules/extban.h"

class ModuleQuietBan
	: public Module
	, public CTCTags::EventListener
{
 private:
	ExtBan::Acting extban;
	bool notifyuser;

 public:
	ModuleQuietBan()
		: Module(VF_VENDOR | VF_OPTCOMMON, "Adds extended ban m which bans specific masks from speaking in a channel.")
		, CTCTags::EventListener(this)
		, extban(this, "mute", 'm')
	{
	}

	void ReadConfig(ConfigStatus& status) override
	{
		ConfigTag* tag = ServerInstance->Config->ConfValue("muteban");
		notifyuser = tag->getBool("notifyuser", true);
	}

	ModResult HandleMessage(User* user, const MessageTarget& target, bool& echo_original)
	{
		if (!IS_LOCAL(user) || target.type != MessageTarget::TYPE_CHANNEL)
			return MOD_RES_PASSTHRU;

		Channel* chan = target.Get<Channel>();
		if (extban.GetStatus(user, chan) == MOD_RES_DENY && chan->GetPrefixValue(user) < VOICE_VALUE)
		{
			if (!notifyuser)
			{
				echo_original = true;
				return MOD_RES_DENY;
			}

			user->WriteNumeric(Numerics::CannotSendTo(chan, "messages", 'm', "mute"));
			return MOD_RES_DENY;
		}

		return MOD_RES_PASSTHRU;
	}

	ModResult OnUserPreMessage(User* user, const MessageTarget& target, MessageDetails& details) override
	{
		return HandleMessage(user, target, details.echo_original);
	}

	ModResult OnUserPreTagMessage(User* user, const MessageTarget& target, CTCTags::TagMessageDetails& details) override
	{
		return HandleMessage(user, target, details.echo_original);
	}
};

MODULE_INIT(ModuleQuietBan)
