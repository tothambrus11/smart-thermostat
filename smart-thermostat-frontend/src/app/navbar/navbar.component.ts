import {Component, Input, OnInit} from '@angular/core';
import {TempService} from '../temp.service';

@Component({
  selector: 'app-navbar',
  templateUrl: './navbar.component.html',
  styleUrls: ['./navbar.component.scss']
})
export class NavbarComponent implements OnInit {

  @Input()
  activeTab!: number;

  constructor(public tempService: TempService) {

  }

  ngOnInit(): void {
  }

}
