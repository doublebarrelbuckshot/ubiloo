import React, { Component } from 'react'
import logo from './logo.svg'
import './App.css'
import openSocket from 'socket.io-client'
import * as MDIcons from 'react-icons/lib/md'

class App extends Component {
  constructor (props) {
    super(props)
    this.state = {
      stallData: []
    }
  }
  componentDidMount () {
    const socket = openSocket('http://localhost:2999')
    socket.on('data', data => {
      console.log('data', data)
      this.setState({stallData: JSON.parse(data)})
    })
  }

  render () {
    const tableData = this.state.stallData.map((item) =>
      <tr>
        <td>
          {item.key}
        </td>
        <td>
          { item.value === 'false' && <MDIcons.MdCancel color='#F62817' size={20} /> }
          { item.value === 'true' && <MDIcons.MdCheckCircle color='green' size={20} /> }
          { (typeof item.value === 'number') && <div>{item.value} </div> }
        </td>
      </tr>
    )

    return (
      <div>
        <h1>
          Ubiloo Status
        </h1>
        <table>
          <thead>
            <tr>
              <th>Stall ID</th>
              <th>Availability</th>
            </tr>
          </thead>
          <tbody>
            {tableData}
          </tbody>
        </table>
      </div>
    )
  }
}

export default App
