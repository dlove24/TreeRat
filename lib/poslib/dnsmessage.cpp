/*
    Posadis - A DNS Server
    Dns Message handling
    Copyright (C) 2002  Meilof Veeningen <meilof@users.sourceforge.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "poslib/sysstl.h"

#include "poslib/dnsmessage.h"
#include "poslib/domainfn.h"
#include "poslib/exception.h"
#include "poslib/rr.h"
#include "poslib/lexfn.h"

message_buff::message_buff() {
  is_static = false;
  msg = NULL;
  len = 0;
  }

message_buff::message_buff (char* _msg, int _len, bool _is_dynamic) {
  is_static = !_is_dynamic;
  msg = _msg;
  len = _len;
  }

message_buff::message_buff (const message_buff& buff) {
  if (buff.is_static) {
    msg = buff.msg;
    len = buff.len;
    is_static = true;
    }

  else {
    if (buff.msg) {
      msg = (char*) memdup (buff.msg, buff.len);
      }

    else {
      msg = NULL;
      }

    len = buff.len;
    is_static = false;
    }
  }

message_buff::~message_buff() {
  if (!is_static) {
    free (msg);
    }

  }

message_buff& message_buff::operator= (const message_buff& buff) {
  if (this != &buff) {
    if (buff.is_static) {
      msg = buff.msg;
      len = buff.len;
      is_static = true;
      }

    else {
      if (buff.msg) {
        msg = (char*) memdup (buff.msg, buff.len);
        }

      else {
        msg = NULL;
        }

      len = buff.len;
      is_static = false;
      }
    }

  return *this;
  }

DnsQuestion::DnsQuestion() {
  QNAME = "";
  QTYPE = 0;
  QCLASS = CLASS_IN;
  }

DnsQuestion::DnsQuestion (const DnsQuestion& q) {
  QNAME = q.QNAME;
  QTYPE = q.QTYPE;
  QCLASS = q.QCLASS;
  }

DnsQuestion& DnsQuestion::operator= (const DnsQuestion& q) {
  if (this != &q) {
    QNAME = q.QNAME;
    QTYPE = q.QTYPE;
    QCLASS = q.QCLASS;
    }

  return *this;
  }

DnsQuestion::DnsQuestion (domainname _QNAME, u_int16 _QTYPE, u_int16 _QCLASS) {
  QNAME = _QNAME;
  QTYPE = _QTYPE;
  QCLASS = _QCLASS;
  }

DnsQuestion::~DnsQuestion() {
  }

DnsRR::DnsRR() {
  NAME = "";
  TYPE = 0;
  CLASS = CLASS_IN;
  TTL = 0;
  RDATA = NULL;
  RDLENGTH = 0;
  }

DnsRR::DnsRR (domainname _NAME, u_int16 _TYPE, u_int16 _CLASS, u_int32 _TTL) {
  NAME = _NAME;
  TYPE = _TYPE;
  CLASS = _CLASS;
  TTL = _TTL;
  RDATA = NULL;
  RDLENGTH = 0;
  }

DnsRR::DnsRR (domainname _NAME, u_int16 _TYPE, u_int16 _CLASS, u_int32 _TTL, uint16_t _RDLENGTH, const char* _RDATA) {
  NAME = _NAME;
  TYPE = _TYPE;
  CLASS = _CLASS;
  TTL = _TTL;
  RDLENGTH = _RDLENGTH;
  RDATA = (char*) memdup (_RDATA, _RDLENGTH);;
  }

DnsRR::DnsRR (const DnsRR& rr) {
  NAME = rr.NAME;
  TYPE = rr.TYPE;
  CLASS = rr.CLASS;
  TTL = rr.TTL;
  RDATA = (char*) memdup (rr.RDATA, rr.RDLENGTH);
  RDLENGTH = rr.RDLENGTH;
  }

DnsRR& DnsRR::operator= (const DnsRR& rr) {
  if (this != &rr) {
    free (RDATA);
    NAME = rr.NAME;
    TYPE = rr.TYPE;
    CLASS = rr.CLASS;
    TTL = rr.TTL;
    RDATA = (char*) memdup (rr.RDATA, rr.RDLENGTH);
    RDLENGTH = rr.RDLENGTH;
    }

  return *this;
  }

DnsRR::~DnsRR() {
  if (RDATA) {
    free (RDATA);
    }
  }

DnsMessage::DnsMessage() {
  ID = 0;
  QR = false;
  OPCODE = 0;
  AA = false;
  TC = false;
  RD = false;
  RA = false;
  Z = 0;
  RCODE = 0;
  }

DnsMessage::~DnsMessage() {
  }

void DnsMessage::read_section (stl_list (DnsRR) & section, int count, message_buff& buff, int& pos) {
  int x;
  domainname dom;
  DnsRR* rr;

  while (--count >= 0) {
    if (pos >= buff.len) {
      throw PException ("Message too small for RR");
      }

    x = dom_comprlen (buff, pos);

    if (pos + x + 10 > buff.len) {
      throw PException ("Message too small for RR");
      }

    dom = domainname (buff, pos);
    section.push_back (DnsRR (dom, uint16_value (buff.msg + pos + x), /* TYPE */
                              uint16_value (buff.msg + pos + x + 2),  /* CLASS */
                              uint32_value (buff.msg + pos + x + 4))); /* TTL */
    pos += x + 10;
    x = uint16_value (buff.msg + pos - 2);
    rr = &*--section.end();
    rr->RDLENGTH = x;
    rr_read (rr->TYPE, rr->RDATA, rr->RDLENGTH, buff, pos, rr->RDLENGTH);
    pos += x;
    }
  }


void DnsMessage::read_from_data (char* data, int len) {
  message_buff buff (data, len);
  int qdc, adc, nsc, arc, t, x, pos = 12;

  if (len < 12) {
    throw PException ("Not enough space for DNS header");
    }

  ID = uint16_value (data);
  QR = data[2] & 128;
  OPCODE = (data[2] & 120) >> 3;
  AA = data[2] & 4;
  TC = data[2] & 2;
  RD = data[2] & 1;
  RA = data[3] & 128;

  Z = (data[3] & 112) >> 3;
  RCODE = data[3] & 15;

  qdc = uint16_value (data + 4);
  adc = uint16_value (data + 6);
  nsc = uint16_value (data + 8);
  arc = uint16_value (data + 10);

  /* read question section */

  for (t = 0; t < qdc; t++) {
    if (pos >= len) {
      throw PException ("Message too small for question item!");
      }

    x = dom_comprlen (buff, pos);

    if (pos + x + 4 > len) {
      throw PException ("Message too small for question item !");
      }

    questions.push_back (DnsQuestion (domainname (buff, pos), uint16_value (data + pos + x), uint16_value (data + pos + x + 2)));
    pos += x;
    pos += 4;

    }

  /* read other sections */
  read_section (answers, adc, buff, pos);
  read_section (authority, nsc, buff, pos);
  read_section (additional, arc, buff, pos);
  };

class PTruncatedException {
  public:
    PTruncatedException() {  }
  };

void DnsMessage::write_section (stl_list (DnsRR) & section, int lenpos, stl_string& message, stl_slist (dom_compr_info) &comprinfo, int maxlen, bool is_additional) {
  stl_list (DnsRR) ::iterator it = section.begin();

  int n = 0, p, x;

  x = message.size();

  while (it != section.end()) {
    dom_write (message, it->NAME.c_str(), comprinfo);
    message.append (uint16_buff (it->TYPE), 2);
    message.append (uint16_buff (it->CLASS), 2);
    message.append (uint32_buff (it->TTL), 4);
    message.append (uint16_buff (it->RDLENGTH), 2);
    p = message.size();
    rr_write (it->TYPE, it->RDATA, it->RDLENGTH, message, comprinfo);

    if (message.size() > (unsigned) maxlen) {
      /* truncate it here */
      message.resize (x);

      if (!is_additional) {
        message[2] |= 2;
        }

      message[lenpos] = n / 256;
      message[lenpos + 1] = n;
      throw PTruncatedException();
      }

    x = message.size();
    message[p - 2] = (x - p) / 256;
    message[p - 1] = (x - p);
    it++;
    n++;
    }

  /* write number of written items */
  message[lenpos] = n / 256;
  message[lenpos + 1] = n;
  }

message_buff DnsMessage::compile (int maxlen) {
  stl_string msg;
  char ch;
  stl_slist (dom_compr_info) comprinfo;

  try {
    msg.append (uint16_buff (ID), 2);

    if (QR) {
      ch = 128;
      }

    else {
      ch = 0;
      }

    ch += OPCODE << 3;

    if (AA) {
      ch += 4;
      }

    if (TC) {
      ch += 2;
      }

    if (RD) {
      ch++;
      }

    msg.append (&ch, 1);

    if (RA) {
      ch = 128;
      }

    else {
      ch = 0;
      }

    ch += Z << 4;
    ch += RCODE;
    msg.append (&ch, 1);
    msg.append (uint16_buff (0), 2);
    msg.append (uint16_buff (0), 2);
    msg.append (uint16_buff (0), 2);
    msg.append (uint16_buff (0), 2);
    //    msg.append(uint16_buff(questions.size()), 2);
    //    msg.append(uint16_buff(answers.size()), 2);
    //    msg.append(uint16_buff(authority.size()), 2);
    //    msg.append(uint16_buff(additional.size()), 2);

    /* write questions */
    stl_list (DnsQuestion) ::iterator it = questions.begin();
    int x, n = 0;

    while (it != questions.end()) {
      x = msg.size();
      dom_write (msg, it->QNAME.c_str(), comprinfo);
      msg.append (uint16_buff (it->QTYPE), 2);
      msg.append (uint16_buff (it->QCLASS), 2);

      if (msg.size() > (unsigned) maxlen) {
        /* truncate it here */
        msg.resize (x);
        msg[2] |= 2;
        msg[4] = n / 256;
        msg[5] = n;
        throw PTruncatedException();
        }

      it++;
      n++;
      }

    /* write number of written items */
    msg[4] = n / 256;
    msg[5] = n;

    /* other sections */
    write_section (answers, 6, msg, comprinfo, maxlen);
    write_section (authority, 8, msg, comprinfo, maxlen);
    write_section (additional, 10, msg, comprinfo, maxlen, true);
    }

  catch (PTruncatedException p) {

    }
  catch (PException p) {
    throw PException ("Dns Message creation failed: ", p);
    }

  int len = msg.size();
  return message_buff ( (char*) memdup ( (char*) msg.c_str(), len), len, true);
  }

char _tmp[4];

char* uint16_buff (uint16_t val) {
  _tmp[0] = val / 256;
  _tmp[1] = val & 255;
  return _tmp;
  }

char* uint32_buff (uint32_t val) {
  _tmp[0] = val / (1 << 24);
  _tmp[1] = val / (1 << 16);
  _tmp[2] = val / (1 << 8);
  _tmp[3] = val;
  return _tmp;
  }

u_int16 uint16_value (char* buff) {
  return buff[0] * 256 + buff[1];
  }

u_int32 uint32_value (char* buff) {
  return uint16_value (buff) * 65536 + uint16_value (buff + 2);
  }

DnsMessage* create_query (domainname QNAME, uint16_t QTYPE, bool RD, uint16_t QCLASS) {
  DnsMessage* ret = new DnsMessage();
  ret->RD = RD;
  ret->questions.push_front (DnsQuestion (QNAME, QTYPE, QCLASS));
  return ret;
  }

a_record get_a_record (DnsMessage* a) {
  return *get_a_records (a, true).begin();
  }

stl_list (a_record) get_a_records (DnsMessage* a, bool fail_if_none) {
  stl_list (a_record) ret;
  a_record rec;
  stl_list (rrdat) res = get_records (a, fail_if_none);
  stl_list (rrdat) ::iterator it = res.begin();

  while (it != res.end()) {
    memcpy (rec.address, it->msg, 4);
    ret.push_back (rec);
    it++;
    }

  return ret;
  }

aaaa_record get_aaaa_record (DnsMessage* a) {
  return *get_aaaa_records (a, true).begin();
  }

stl_list (aaaa_record) get_aaaa_records (DnsMessage* a, bool fail_if_none) {
  stl_list (aaaa_record) ret;
  aaaa_record rec;
  stl_list (rrdat) res = get_records (a, fail_if_none, DNS_TYPE_AAAA);
  stl_list (rrdat) ::iterator it = res.begin();

  while (it != res.end()) {
    memcpy (rec.address, it->msg, 16);
    ret.push_back (rec);
    it++;
    }

  return ret;
  }

mx_record get_mx_record (DnsMessage* a) {
  return *get_mx_records (a, true).begin();
  }
stl_list (mx_record) get_mx_records (DnsMessage* a, bool fail_if_none) {
  stl_list (mx_record) ret;
  mx_record rec;
  stl_list (rrdat) res = get_records (a, fail_if_none);
  stl_list (rrdat) ::iterator it = res.begin();

  while (it != res.end()) {
    rec.preference = rr_getshort (it->msg, DNS_TYPE_MX, 0);
    rec.server = rr_getdomain (it->msg, DNS_TYPE_MX, 1);
    ret.push_back (rec);
    it++;
    }

  return ret;
  }


domainname get_ns_record (DnsMessage* a) {
  return *get_ns_records (a, true).begin();
  }
stl_list (domainname) get_ns_records (DnsMessage* a, bool fail_if_none) {
  stl_list (domainname) ret;
  stl_list (rrdat) res = get_records (a, fail_if_none);
  stl_list (rrdat) ::iterator it = res.begin();

  while (it != res.end()) {
    ret.push_back (rr_getdomain (it->msg, DNS_TYPE_NS, 0));
    it++;
    }

  return ret;
  }

domainname get_ptr_record (DnsMessage* a) {
  return *get_ptr_records (a, true).begin();
  }
stl_list (domainname) get_ptr_records (DnsMessage* a, bool fail_if_none) {
  stl_list (domainname) ret;
  stl_list (rrdat) res = get_records (a, fail_if_none);
  stl_list (rrdat) ::iterator it = res.begin();

  while (it != res.end()) {
    ret.push_back (rr_getdomain (it->msg, DNS_TYPE_PTR, 0));
    it++;
    }

  return ret;
  }

rrdat::rrdat (uint16_t _type, uint16_t _len, char* _msg) {
  type = _type;
  len = _len;
  msg = _msg;
  }

stl_list (rrdat) i_get_records (DnsMessage* a, bool fail_if_none, bool follow_cname, int reclevel, domainname& dname, uint16_t qtype, stl_list (domainname) *fcn) {
  stl_list (rrdat) ret;
  domainname dm;

  if (reclevel < 0) {
    throw PException ("CNAME recursion level reached");
    }

  /* look for records */
  stl_list (DnsRR) ::iterator it = a->answers.begin();

  while (it != a->answers.end()) {
    if (it->NAME == dname) {
      if (it->TYPE == DNS_TYPE_CNAME && follow_cname && qtype != DNS_TYPE_CNAME) {
        dm = domainname (true, it->RDATA);

        if (fcn) {
          fcn->push_back (dm);
          }

        return i_get_records (a, fail_if_none, true, --reclevel, dm, qtype, fcn);
        }

      else if (it->TYPE == qtype || qtype == QTYPE_ALL) {
        ret.push_back (rrdat (it->TYPE, it->RDLENGTH, it->RDATA));
        }
      }

    it++;
    }

  if (fail_if_none && ret.begin() == ret.end()) {
    throw PException ("No such data available");
    }

  return ret;
  }

stl_list (rrdat) get_records (DnsMessage* a, bool fail_if_none, bool follow_cname, stl_list (domainname) *fcn) {
  if (a->RCODE != RCODE_NOERROR) {
    throw PException (true, "Query returned error: %s\n", str_rcode (a->RCODE).c_str());
    }

  if (a->questions.begin() == a->questions.end()) {
    throw PException ("No question item in message");
    }

  return i_get_records (a, fail_if_none, follow_cname, 10, \
                        a->questions.begin()->QNAME, \
                        a->questions.begin()->QTYPE, fcn);
  }

bool has_rrset (stl_list (DnsRR) &rrlist, domainname& QNAME, uint16_t QTYPE) {
  stl_list (DnsRR) ::iterator it = rrlist.begin();

  while (it != rrlist.end()) {
    if (it->NAME == QNAME && answers_qtype (it->TYPE, QTYPE)) {
      return true;
      }

    it++;
    }

  return false;
  }


bool has_parental_rrset (stl_list (DnsRR) & section, domainname& qname, uint16_t type) {
  stl_list (DnsRR) ::iterator it = section.begin();

  while (it != section.end()) {
    if (it->TYPE == type && qname >= it->NAME) {
      return true;
      }

    it++;
    }

  return false;
  }

_answer_type check_answer_type (DnsMessage* msg, domainname& qname, uint16_t qtype) {
  if (msg->RCODE != RCODE_NOERROR && msg->RCODE != RCODE_NXDOMAIN) {
    return A_ERROR;
    }

  if (qtype != DNS_TYPE_CNAME && has_rrset (msg->answers, qname, DNS_TYPE_CNAME)) {
    return A_CNAME;
    }

  if (msg->RCODE == RCODE_NXDOMAIN) {
    return A_NXDOMAIN;
    }

  if (has_rrset (msg->answers, qname, qtype)) {
    return A_ANSWER;
    }

  if (has_parental_rrset (msg->authority, qname, DNS_TYPE_NS) &&
      !has_parental_rrset (msg->authority, qname, DNS_TYPE_SOA)) {
    return A_REFERRAL;
    }

  return A_NODATA;
  }
