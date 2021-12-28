import * as React from 'react';

import FormGroup from '@mui/material/FormGroup';
import FormControlLabel from '@mui/material/FormControlLabel';
import FormControl from '@mui/material/FormControl';
import FormLabel from '@mui/material/FormLabel';

interface ReferProp {
    onClick: Function,
    data: {
        comment: string,
        dtype: string,
        index: number,
        mtype: number,
        name: string,
        premission: number,
        value: boolean
    }
}

export default class ItemRefer extends React.Component<ReferProp> {
    constructor(props: ReferProp) {
        super(props)
    }
    render() {
        return (
            <FormControl component="fieldset">
                <FormGroup aria-label="position" row>
                </FormGroup>
            </FormControl>
        );
    }
}