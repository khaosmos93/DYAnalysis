void misctools(void) { }

//tool that converts float to string - opposite to atof
const char* fota(float input) {

   std::ostringstream pprint;
   pprint.str("");
   pprint<<input;
   return pprint.str().c_str();

}
