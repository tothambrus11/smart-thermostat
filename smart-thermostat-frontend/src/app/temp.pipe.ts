import {Pipe, PipeTransform} from '@angular/core';

@Pipe({
  name: 'temp'
})
export class TempPipe implements PipeTransform {

  transform(value: number, digits = 1): string {
    let str = Math.round(value * 10 ** digits).toString();
    return str.substr(0, str.length - digits) + ',' + str.substr(str.length - digits, str.length);
  }

}
