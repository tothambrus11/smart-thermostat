import { Component, OnInit } from '@angular/core';
import {Router} from "@angular/router";
import {TempService} from '../temp.service';

@Component({
  selector: 'app-logout',
  templateUrl: './logout.component.html',
  styleUrls: ['./logout.component.scss']
})
export class LogoutComponent implements OnInit {

  constructor(private router: Router) { }

  ngOnInit(): void {
    // todo logout
    TempService.logout();
    this.router.navigate(["/login"])
  }

}
