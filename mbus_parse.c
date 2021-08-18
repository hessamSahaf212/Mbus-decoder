/**
 * parsing Mbus decoder
 * 
 */

#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <uchar.h>
#include <io.h>

#include "mbus-protocol.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
	int len;

	// fixed: 2 counter
		//68 13 13 68 header of RSP_UD telegram(L - Field = 13h = 19d)
		//08 05 73 C field = 08h(RSP_UD), address 5, CI field = 73h(fixed, LSByte first)
		//78 56 34 12 identification number = 12345678
		//0A transmission counter = 0Ah = 10d
		//00 status 00h: counters coded BCD, actual values, no errors
		//E9 7E Type & Unit : medium water, unit1 = 1l, unit2 = 1l (same, but historic)
		//01 00 00 00 counter 1 = 1l (actual value)
		//35 01 00 00 counter 2 = 135 l(historic value)
		//3C 16 checksum and stop sign

	//unsigned char buf[] = { 0x68 ,0x13 ,0x13 ,0x68 ,0x08 ,0x05 ,0x73 ,0x78 ,0x56 ,0x34 ,0x12 ,0x0A ,0x00 ,0xE9 ,0x7E ,0x01 ,0x00 ,0x00 ,0x00 ,0x35 ,0x01 ,0x00 ,0x00 ,0x3C ,0x16  };


	// variable:
		//68 1F 1F 68 header of RSP_UD telegram(length 1Fh = 31d bytes)
		//08 02 72 C field = 08 (RSP), address 2, CI field 72H(var., LSByte first)
		//78 56 34 12 identification number = 12345678
		//24 40 01 07 manufacturer ID = 4024h(PAD in EN 61107), generation 1, water
		//55 00 00 00 TC = 55h = 85d, Status = 00h, Signature = 0000h
		//03 13 15 31 00 Data block 1: unit 0, storage No 0, no tariff, instantaneous volume,
		//12565 l(24 bit integer)
		//DA 02 3B 13 01 Data block 2 : unit 0, storage No 5, no tariff, maximum volume flow,
		//113 l / h(4 digit BCD)
		//8B 60 04 37 18 02 Data block 3 : unit 1, storage No 0, tariff 2, instantaneous energy,
		//218, 37 kWh(6 digit BCD)
		//18 16 checksum and stopsign
		// https://m-bus.com/documentation-wired/06-application-layer
	//unsigned char buf[] = { 0x68 ,0x1F ,0x1F ,0x68 ,0x08 ,0x02 ,0x72 ,0x78 ,0x56 ,0x34 ,0x12 ,0x24 ,0x40 ,0x01 ,0x07 ,0x55 ,0x00 ,0x00 ,0x00 ,0x03 ,0x13 ,0x15 ,0x31 ,0x00 ,0xDA ,0x02 ,0x3B ,0x13 ,0x01 ,0x8B ,0x60 ,0x04 ,0x37 ,0x18 ,0x02 ,0x18 ,0x16 };


	/*unsigned char buf[] = { 0x68 ,0x71 ,0x71 ,0x68 ,0x08 ,0x96 ,0x72 ,0x55 ,0x64 ,0x66 ,0x10 ,0x49 ,0x6a ,0x88 ,0x04 ,0x07 ,0x00 ,0x00 ,0x00 ,0x0c ,0x78 ,0x55 ,0x64 ,0x66
		, 0x10, 0x04, 0x06, 0x00, 0x00, 0x00, 0x00, 0x82, 0x04, 0x6c, 0xa1, 0x21, 0xc2, 0x84, 0x00, 0x6c, 0xff, 0xff, 0x84, 0x04, 0x06, 0x00, 0x00, 0x00
		, 0x00, 0xc4, 0x84, 0x00, 0x06, 0x00, 0x00, 0x00, 0x80, 0x82, 0x0a, 0x6c, 0xa1, 0x26, 0x84, 0x0a, 0x06, 0x00, 0x00, 0x00, 0x00, 0x04, 0x13, 0x03
		, 0x00, 0x00, 0x00, 0x02, 0x59, 0x4c, 0x08, 0x02, 0x5d, 0x1f, 0x08, 0x02, 0x61, 0x2d, 0x00, 0x04, 0x2d, 0x00, 0x00, 0x00, 0x00, 0x04, 0x3b, 0x00
		, 0x00, 0x00, 0x00, 0x04, 0x6d, 0x2b, 0x12, 0xa6, 0x26, 0x04, 0x26, 0xd6, 0x08, 0x00, 0x00, 0x02, 0xfd, 0x17, 0x00, 0x00, 0x1f, 0x5b, 0x16 };*/

	//unsigned char buf[] = { 0x68 ,0x71 ,0x71 ,0x68 ,0x08 ,0x96 ,0x72 ,0x55 ,0x64 ,0x66 ,0x10 ,0x49 ,0x6a ,0x88 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x0c ,0x78 ,
	//	0x55 ,0x64 ,0x66 ,0x10 ,0x04 ,0x06 ,0x00 ,0x00 ,0x00 ,0x00 ,0x82 ,0x04 ,0x6c ,0xa1 ,0x21 ,0xc2 ,0x84 ,0x00 ,0x6c ,0xff ,0xff ,0x84 ,0x04 ,0x06 
	//	,0x00 ,0x00 ,0x00 ,0x00 ,0xc4 ,0x84 ,0x00 ,0x06 ,0x00 ,0x00 ,0x00 ,0x80 ,0x82 ,0x0a ,0x6c ,0xa1 ,0x26 ,0x84 ,0x0a ,0x06 ,0x00 ,0x00 ,0x00 ,0x00 
	//	,0x04 ,0x13 ,0x03 ,0x00 ,0x00 ,0x00 ,0x02 ,0x59 ,0x88 ,0x07 ,0x02 ,0x5d ,0x15 ,0x08 ,0x02 ,0x61 ,0x00 ,0x00 ,0x04 ,0x2d ,0x00 ,0x00 ,0x00 ,0x00 
	//	,0x04 ,0x3b ,0x00 ,0x00 ,0x00 ,0x00 ,0x04 ,0x6d ,0x16 ,0x12 ,0xa6 ,0x26 ,0x04 ,0x26 ,0xd6 ,0x08 ,0x00 ,0x00 ,0x02 ,0xfd ,0x17 ,0x00 ,0x00 ,0x1f 
	//	,0x43 ,0x16 };



	// Ack:
	//unsigned char buf[] = { 0xE5 };

	unsigned char buf[] = { 0x68, 0x71, 0x71, 0x68, 0x08, 0x96, 0x72, 0x55, 0x64, 0x66, 0x10, 0x49, 0x6a, 0x88, 0x04, 0x14, 0x00, 0x00,
		0x00, 0x0c, 0x78, 0x55, 0x64, 0x66, 0x10, 0x04, 0x06, 0x00, 0x00, 0x00, 0x00, 0x82, 0x04, 0x6c, 0xa1, 0x21, 0xc2, 0x84, 0x00, 
		0x6c, 0xff, 0xff, 0x84, 0x04, 0x06, 0x00, 0x00, 0x00, 0x00, 0xc4, 0x84, 0x00, 0x06, 0x00, 0x00, 0x00, 0x80, 0x82, 0x0a, 0x6c,
		0xa1, 0x28, 0x84, 0x0a, 0x06, 0x00, 0x00, 0x00, 0x00, 0x04, 0x13, 0x06, 0x00, 0x00, 0x00, 0x02, 0x59, 0x42, 0x09, 0x02, 0x5d,
		0x3b, 0x09, 0x02, 0x61, 0x07, 0x00, 0x04, 0x2d, 0x00, 0x00, 0x00, 0x00, 0x04, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x04, 0x6d, 0x24,
		0x08, 0xad, 0x28, 0x04, 0x26, 0x2c, 0x0f, 0x00, 0x00, 0x02, 0xfd, 0x17, 0x00, 0x00, 0x1f, 0xb0, 0x16 };

	



	mbus_frame reply;
	mbus_frame_data frame_data;
	mbus_data_fixed data_fix;
	mbus_data_variable data_var;
	mbus_data_variable_header data_var_header;
	mbus_data_record data_var_record;
	char* xml_result = NULL;
	char* jSon_result = NULL;

	len = sizeof(buf);

	memset(&reply, 0, sizeof(reply));
	memset(&frame_data, 0, sizeof(frame_data));
	memset(&data_fix, 0, sizeof(data_fix));
	memset(&data_var, 0, sizeof(data_var));
	memset(&data_var_record, 0, sizeof(data_var_record));
	memset(&data_var_header, 0, sizeof(data_var_header));

	mbus_parse(&reply, buf, len);   // frame ::reply 
	mbus_frame_verify(&reply);      // to verify
	unsigned char check_sum1 = calc_checksum(&reply);
	printf("\n checksum:      %.2X  ", check_sum1);


	//------------------------------------------------------------------------------
	/// Verify that parsed frame is a valid M-bus frame. 
	//
	// Possible checks:
	//
	// 1) frame type
	// 2) Start/stop bytes
	// 3) control field
	// 4) length field and actual data size
	// 5) checksum
	//
	//------------------------------------------------------------------------------

	if (mbus_frame_verify(&reply) == 0) {
		printf(" \n valid M-bus frame");
	}
	else {
		printf(" \n invalid M-bus frame\n");

	}

	//printf("\n checksum %.10X \n ", calc_length(&reply));



	if (reply.control_information  == 0x73 || reply.control_information == 0x77) {                //// data fix

		printf("\n Frame Data Type: fixed ");
		printf("\n Telegram Size:  %d ", reply.data_size);
		printf("\n buffer Size:  %d \n\n", len);

		mbus_data_fixed_parse(&reply, &data_fix);
		int frame_cnt;
		frame_cnt = mbus_frame_data_parse(&reply, &data_fix);
		mbus_frame_data_parse(&reply, &frame_data);

		mbus_frame_data_print(&data_fix);
		mbus_frame_data_xml(&frame_data);
		mbus_data_fixed_print(&data_fix);

	}

	if (reply.control_information == 0x72 || reply.control_information == 0x76) {                  /////////data variable
		printf("\n Frame Data Type:  Variable ");
		printf("\n Telegram Size: %d \n ", reply.data_size);
		printf("\n buffer Size:  %d \n", len);


		mbus_data_variable_parse(&reply, &data_var);
		int frame_cnt;
		frame_cnt = mbus_frame_data_parse(&reply , &frame_data);
		//int record_cnt;
		// ecord_cnt = 
		//mbus_frame_data_print(&data_var);
		//mbus_data_variable_print(&data_var);

		//// xml

		//mbus_frame_data_xml(&frame_data);

		//struct tm* tim11;
		//mbus_data_tm_decode(&tim11, &data_var_record, 2);
		//const char* label11 = "l ";
		//const char* buff11 = "b ";
		//size_t len11 = 2;
		//mbus_hex_dump(&label11, &buff11, len11);

		//mbus_data_record_append(&data_var, &data_var_record);
		// printf("\n record     %.s", *mbus_data_record_decode(&data_var_record));


		mbus_data_record_decode(&data_var_record);
		mbus_frame_xml(&reply);
		mbus_data_variable_header_print(&data_var.header);

		//------------------------------------------------------------------------------
		/// Generate XML for a single variable-length data record
		//------------------------------------------------------------------------------
		//mbus_data_variable_record_xml(&data_var_record, int record_cnt, int frame_cnt, &data_var_header);


		//printf("\n\n%s: Medium1  = %s\n", __FUNCSIG__, mbus_data_record_decode(&data_var_record));
		// const char  record_decode[768] = mbus_data_record_decode(&data_var_record);
		//printf("\n identification number: %.2X \n ", mbus_data_int_decode(&data_var.header.id_bcd , sizeof(data_var.header.id_bcd)));
		//printf("\n data val %.2X \n ", data_fix.tx_cnt);
		//printf("\n status :  %.2X \n ", data_fix.status);
		//printf("\n data val %.2X \n ", data_fix.cnt1_type);
		//printf("\n data val %.2X \n ", data_fix.cnt2_type);
		//printf("\n counter 1: %.2X \n ", mbus_data_int_decode(&data_fix.cnt1_val, sizeof(data_fix.id_bcd)));
		//printf("\n counter 2: %.2X \n ", mbus_data_int_decode(&data_fix.cnt2_val, sizeof(data_fix.id_bcd)));


	}



	// long long data_val = mbus_data_int_decode( &reply.data , &reply.data_size);

	//mbus_frame_verify(&reply);

	//mbus_frame_data_parse(&reply, &frame_data);
	/*mbus_frame_print(&reply);*/

	if ((xml_result = mbus_frame_data_xml(&frame_data)) == NULL)
	{
		fprintf(stderr, "Failed to generate XML representation of MBUS frame: %s\n", mbus_error_str());
		return 1;
	}
	printf("%s", xml_result);
	free(xml_result);

	// json result h.

	if ((jSon_result = mbus_frame_data_jSon(&frame_data)) == NULL)
	{
		fprintf(stderr, "Failed to generate jSon representation of MBUS frame: %s\n", mbus_error_str());
		return 1;
	}
	printf("%s", jSon_result);
	free(jSon_result);

	return 0;
}
