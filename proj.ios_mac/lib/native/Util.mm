//
//  Util.mm
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/12/08.
//
//

#import "AppController.h"
#include "lib/native/Util.h"

namespace lib {

namespace native {

void Util::closeIme() { [AppController closeIme]; }
}
}