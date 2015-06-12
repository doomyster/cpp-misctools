/*
    Copyright © 2013-2015 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cpptTest-encoding.h"
#include "cppmt-string.h"
#include <iostream>
#include <algorithm>
#include <cstring>

CPPUNIT_TEST_SUITE_REGISTRATION( cpptTestEncoding );

static std::string long_data = "Un bon petit diable a la fleur de l'age,\n"             "La jambe legere et l'oeil polisson,\n"
			       "Et la bouche pleine de joyeux ramages,\n"               "Allait a la chasse aux papillons.\n"
			       "\n"
			       "Comme il atteignait l'oree du village,\n"               "Filant sa quenouille, il vit Cendrillon,\n"
			       "Il lui dit : <<Bonjour, que Dieu te menage,\n"          "J' t'emmene a la chasse aux papillons.>>\n"
			       "\n"
			       "Cendrillon ravi' de quitter sa cage,\n"                 "Met sa robe neuve et ses botillons ;\n"
			       "Et bras d'ssus bras d'ssous vers les frais bocages\n"   "Ils vont a la chasse aux papillons.\n"
			       "\n"
			       "Il ne savait pas que sous les ombrages,\n"              "Se cachait l'amour et son aiguillon,\n"
			       "Et qu'il transperçait les coeurs de leur age,\n"         "Les cœurs des chasseurs de papillons.\n"
			       "\n"
			       "\n"
			       "Quand il se fit tendre, ell’ lui dit : <<J' presage\n"  "Qu' c'est pas dans les plis de mon cotillon,\n"
			       "Ni dans l'echancrure de mon corsage,\n"                 "Qu'on va a la chasse aux papillons.>>\n"
			       "\n"
			       "Sur sa bouche en feu qui criait : <<Sois sage !>>\n"    "Il posa sa bouche en guis’ de baillon,\n"
			       "Et c'fut l'plus charmant des remu’-menage\n"            "Qu'on ait vu d' memoir' de papillon\n"
			       "\n"
			       "Un volcan dans l'ame, i’ r'vinr’nt au village,\n"       "En se promettant d'aller des millions,\n"
			       "Des milliards de fois, et mem' davantage,\n"            "Ensemble a la chasse aux papillons.\n"
			       "\n"
			       "Mais tant qu'ils s'aim'ront, tant que les nuages,\n"    "Porteurs de chagrins, les epargneront,\n"
			       "Il f'ra bon voler dans les frais bocages,\n"            "Ils f'ront pas la chasse aux papillons.\n";

static std::string long_data_b64 = "VW4gYm9uIHBldGl0IGRpYWJsZSBhIGxhIGZsZXVyIGRlIGwnYWdlLApMYSBqYW1iZSBsZWdlcmUg"
				   "ZXQgbCdvZWlsIHBvbGlzc29uLApFdCBsYSBib3VjaGUgcGxlaW5lIGRlIGpveWV1eCByYW1hZ2Vz"
				   "LApBbGxhaXQgYSBsYSBjaGFzc2UgYXV4IHBhcGlsbG9ucy4KCkNvbW1lIGlsIGF0dGVpZ25haXQg"
				   "bCdvcmVlIGR1IHZpbGxhZ2UsCkZpbGFudCBzYSBxdWVub3VpbGxlLCBpbCB2aXQgQ2VuZHJpbGxv"
				   "biwKSWwgbHVpIGRpdCA6IDw8Qm9uam91ciwgcXVlIERpZXUgdGUgbWVuYWdlLApKJyB0J2VtbWVu"
				   "ZSBhIGxhIGNoYXNzZSBhdXggcGFwaWxsb25zLj4+CgpDZW5kcmlsbG9uIHJhdmknIGRlIHF1aXR0"
				   "ZXIgc2EgY2FnZSwKTWV0IHNhIHJvYmUgbmV1dmUgZXQgc2VzIGJvdGlsbG9ucyA7CkV0IGJyYXMg"
				   "ZCdzc3VzIGJyYXMgZCdzc291cyB2ZXJzIGxlcyBmcmFpcyBib2NhZ2VzCklscyB2b250IGEgbGEg"
				   "Y2hhc3NlIGF1eCBwYXBpbGxvbnMuCgpJbCBuZSBzYXZhaXQgcGFzIHF1ZSBzb3VzIGxlcyBvbWJy"
				   "YWdlcywKU2UgY2FjaGFpdCBsJ2Ftb3VyIGV0IHNvbiBhaWd1aWxsb24sCkV0IHF1J2lsIHRyYW5z"
				   "cGVyw6dhaXQgbGVzIGNvZXVycyBkZSBsZXVyIGFnZSwKTGVzIGPFk3VycyBkZXMgY2hhc3NldXJz"
				   "IGRlIHBhcGlsbG9ucy4KCgpRdWFuZCBpbCBzZSBmaXQgdGVuZHJlLCBlbGzigJkgbHVpIGRpdCA6"
				   "IDw8SicgcHJlc2FnZQpRdScgYydlc3QgcGFzIGRhbnMgbGVzIHBsaXMgZGUgbW9uIGNvdGlsbG9u"
				   "LApOaSBkYW5zIGwnZWNoYW5jcnVyZSBkZSBtb24gY29yc2FnZSwKUXUnb24gdmEgYSBsYSBjaGFz"
				   "c2UgYXV4IHBhcGlsbG9ucy4+PgoKU3VyIHNhIGJvdWNoZSBlbiBmZXUgcXVpIGNyaWFpdCA6IDw8"
				   "U29pcyBzYWdlICE+PgpJbCBwb3NhIHNhIGJvdWNoZSBlbiBndWlz4oCZIGRlIGJhaWxsb24sCkV0"
				   "IGMnZnV0IGwncGx1cyBjaGFybWFudCBkZXMgcmVtdeKAmS1tZW5hZ2UKUXUnb24gYWl0IHZ1IGQn"
				   "IG1lbW9pcicgZGUgcGFwaWxsb24KClVuIHZvbGNhbiBkYW5zIGwnYW1lLCBp4oCZIHIndmlucuKA"
				   "mW50IGF1IHZpbGxhZ2UsCkVuIHNlIHByb21ldHRhbnQgZCdhbGxlciBkZXMgbWlsbGlvbnMsCkRl"
				   "cyBtaWxsaWFyZHMgZGUgZm9pcywgZXQgbWVtJyBkYXZhbnRhZ2UsCkVuc2VtYmxlIGEgbGEgY2hh"
				   "c3NlIGF1eCBwYXBpbGxvbnMuCgpNYWlzIHRhbnQgcXUnaWxzIHMnYWltJ3JvbnQsIHRhbnQgcXVl"
				   "IGxlcyBudWFnZXMsClBvcnRldXJzIGRlIGNoYWdyaW5zLCBsZXMgZXBhcmduZXJvbnQsCklsIGYn"
				   "cmEgYm9uIHZvbGVyIGRhbnMgbGVzIGZyYWlzIGJvY2FnZXMsCklscyBmJ3JvbnQgcGFzIGxhIGNo"
				   "YXNzZSBhdXggcGFwaWxsb25zLgo=";

void cpptTestEncoding::testBase64()
{
	typedef std::map<std::string, std::string> test_t;
	test_t test_data;

	test_data["Coucou"]   = "Q291Y291";
	test_data["Coucou\n"] = "Q291Y291Cg==";
	test_data[long_data]  = long_data_b64;

	for (test_t::const_iterator it = test_data.begin(), e = test_data.end(); it != e; ++it) {
		// Test interface 0:
		{
			std::string s = cppmt::base64::encode(it->first.c_str(), it->first.size());
			s.erase(remove(s.begin(), s.end(), '\n'), s.end());
			CPPUNIT_ASSERT_MESSAGE("Item <" + it->first + ">: b64 encoding failure [interface 0].", s == it->second);
		}

		// Test interface 1:
		{
			std::string s = cppmt::base64::encode(it->first);
			s.erase(remove(s.begin(), s.end(), '\n'), s.end());
			CPPUNIT_ASSERT_MESSAGE("Item <" + it->first + ">: b64 encoding failure [interface 1].", s == it->second);
		}

		// Test interface 2:
		{
			std::string o;
			cppmt::base64::encode(it->first, o);
			o.erase(remove(o.begin(), o.end(), '\n'), o.end());
			CPPUNIT_ASSERT_MESSAGE("Item <" + it->first + ">: b64 encoding failure [interface 2].", o == it->second);
		}

		// Test interface 3:
		{
			std::stringstream ss_i(it->first);
			std::stringstream ss_o;
			cppmt::base64::encode(ss_i, ss_o);
			std::string o = ss_o.str();
			o.erase(remove(o.begin(), o.end(), '\n'), o.end());
			CPPUNIT_ASSERT_MESSAGE("Item <" + it->first + ">: b64 encoding failure [interface 3].", o == it->second);
		}
	}

	// Test binary; encode and decode:
	// {
		float src[512] = {};
		float dst[512] = {};
		for (int i=0; i<512; ++i) {
			src[i] = rand() / (float)(RAND_MAX);
		}

		std::string o0, o1, o2, o3;
		// Test interface 0:
		{
			std::string o = cppmt::base64::encode(reinterpret_cast<char*>(src), sizeof(float) * 512);
			for (int i=0; i<o.size(); ++i) {
				CPPUNIT_ASSERT(::isprint(o.data()[i]) || o.data()[i] == '\n');
			}
			o.erase(remove(o.begin(), o.end(), '\n'), o.end());
			o0 = o;
			
			std::string r = cppmt::base64::decode(o);
			CPPUNIT_ASSERT(r.size() == sizeof(float) * 512);
			std::memcpy(reinterpret_cast<char*>(dst), r.data(), r.size());

			for (int i=0; i<512; ++i) {
				CPPUNIT_ASSERT(src[i] == dst[i]);
			}
		}

		// Test interface 1:
		{
			std::string s_src(reinterpret_cast<char*>(src), sizeof(float) * 512);
			std::string o = cppmt::base64::encode(s_src);
			for (int i=0; i<o.size(); ++i) {
				CPPUNIT_ASSERT(::isprint(o.data()[i]) || o.data()[i] == '\n');
			}
			o.erase(remove(o.begin(), o.end(), '\n'), o.end());
			o1 = o;
			
			std::string r = cppmt::base64::decode(o);
			CPPUNIT_ASSERT(r.size() == sizeof(float) * 512);
			std::memcpy(reinterpret_cast<char*>(dst), r.data(), r.size());

			for (int i=0; i<512; ++i) {
				CPPUNIT_ASSERT(src[i] == dst[i]);
			}
		}

		// Test interface 2:
		{
			std::string s_src(reinterpret_cast<char*>(src), sizeof(float) * 512);
			std::string o;
			cppmt::base64::encode(s_src, o);
			for (int i=0; i<o.size(); ++i) {
				CPPUNIT_ASSERT(::isprint(o.data()[i]) || o.data()[i] == '\n');
			}
			o.erase(remove(o.begin(), o.end(), '\n'), o.end());
			o2 = o;
			
			std::string r = cppmt::base64::decode(o);
			CPPUNIT_ASSERT(r.size() == sizeof(float) * 512);
			std::memcpy(reinterpret_cast<char*>(dst), r.data(), r.size());

			for (int i=0; i<512; ++i) {
				CPPUNIT_ASSERT(src[i] == dst[i]);
			}
		}

		// Test interface 3:
		{
			std::stringstream ss_i;
			std::stringstream ss_o;

			ss_i.write(reinterpret_cast<char*>(src), sizeof(float) * 512);
			cppmt::base64::encode(ss_i, ss_o);
			std::string o = ss_o.str();
			for (int i=0; i<o.size(); ++i) {
				CPPUNIT_ASSERT(::isprint(o.data()[i]) || o.data()[i] == '\n');
			}
			o.erase(remove(o.begin(), o.end(), '\n'), o.end());
			o3 = o;
			
			std::string r = cppmt::base64::decode(o);
			CPPUNIT_ASSERT(r.size() == sizeof(float) * 512);
			std::memcpy(reinterpret_cast<char*>(dst), r.data(), r.size());

			for (int i=0; i<512; ++i) {
				CPPUNIT_ASSERT(src[i] == dst[i]);
			}
		}

		CPPUNIT_ASSERT(o0 == o1 && o0 == o2 && o0 == o3);
	// }
}


