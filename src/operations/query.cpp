/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023 Source2ZE
 * Original code from SourceMod
 * Copyright (C) 2004-2014 AlliedModders LLC
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "query.h"
#include "tier0/dbg.h"
#include "../mysql_result.h"

TQueryOp::~TQueryOp()
{
	delete m_pQuery;
}

void TQueryOp::RunThreadPart()
{
	auto pDatabase = m_pCon->GetDatabase();
	m_szError[0] = '\0';
	if (mysql_query(pDatabase, m_szQuery.c_str()))
	{
		// CR_SERVER_GONE_ERROR (2006) or CR_SERVER_LOST (2013) — try to reconnect once
		unsigned int err = mysql_errno(pDatabase);
		if ((err == CR_SERVER_GONE_ERROR || err == CR_SERVER_LOST) && m_pCon->ReconnectSync())
		{
			pDatabase = m_pCon->GetDatabase();
			if (!mysql_query(pDatabase, m_szQuery.c_str()))
			{
				if (mysql_field_count(pDatabase))
					m_res = mysql_store_result(pDatabase);
				return;
			}
		}
		V_snprintf(m_szError, sizeof m_szError, "MySQL query error: %s\n", mysql_error(pDatabase));
		return;
	}

	if (mysql_field_count(pDatabase))
		m_res = mysql_store_result(pDatabase);
}

void TQueryOp::RunThinkPart()
{
	if(m_szError[0])
	{
		ConMsg("%s\n", m_szError);
		return;
	}

	m_pQuery = new CMySQLQuery(m_pCon, m_res);
	m_callback(m_pQuery);
}

void TQueryOp::CancelThinkPart()
{
	mysql_close(m_pCon->GetDatabase());
	m_pCon->SetDatabase(nullptr);
}
